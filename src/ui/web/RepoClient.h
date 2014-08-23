/* 
 * File:   RepoClient.h
 * Author: johadunf
 *
 * Created on May 20, 2014, 6:52 PM
 */

#ifndef REPOCLIENT_H
#define	REPOCLIENT_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "RepositoryInfo.h"
#include "RepoBrowserFactory.h"
#include "IRepoBrowser.h"
#include "IRepoObserver.h"
#include "HttpClient.h"


typedef std::vector<std::string> StringVector;
typedef std::pair<RepositoryInfo, std::string> RepoFile;
typedef std::map<int, RepoFile> FileCache;

class RepoCommand {
public:
    std::string cmd;
    StringVector args;
};

class RepoClient {
public:

    RepoClient() {
        globalFileIndex = 0;
    }

    /**
     * Process client input for handling remote rev files
     * 
     * @param command   Command to process
     * @return          Returns true if command was relevant, otherwise false
     */
    bool processCommand(std::vector<RepositoryInfo> repositories, std::string cmd) {
        boost::trim(cmd);
        if(cmd.size() == 0){
            return false;
        }
        
        RepoCommand rc;
        boost::split(rc.args, cmd, boost::is_any_of(" "));
        rc.cmd = pop_front(rc.args);

//        std::cout << "cmd: " << rc.cmd << ", args: ";
//        BOOST_FOREACH(std::string a, rc.args) {
//            std::cout << a << ", ";
//        }
//        std::cout << "\n";

        if (rc.cmd == "repo_list") {
            // reset global file index and start a new
            globalFileIndex = 0;

            BOOST_FOREACH(RepositoryInfo repo, repositories) {
                IRepoBrowser *browser = RepoBrowserFactory::getRepoBrowser(repo); // may return null
                if (browser) {
                    HttpResponse httpResponse = browser->getIndex();
                    httpResponse.data = pushIndex(repo, httpResponse.data);
                    repoObserver->notifyGetIndexComplete(httpResponse, repo);
                }
            }
            return true;

        } else if (rc.cmd == "repo_get") {

            // make sure we have a cache set
            if (fileCache.size() <= 0) {
                repoObserver->notifyError("Cache not set, initializing now... Try download again when complete!");
                processCommand(repositories, "repo_list");
                return true;
            }

            // check syntax
            std::string error = "Valid syntax to download a file is:\n 1) repo-get <file index>\n 2) repo-get <repository>.<filename>";
            if (rc.args.size() != 1) {
                repoObserver->notifyError(error);
                return true;
            }
            int index;
            try {
                index = boost::lexical_cast<int>(rc.args.at(0));

            } catch (std::exception& e) {
                repoObserver->notifyError(error);
                return true;
            }

            // check that index exist
            if (!fileCache.count(index)) {
                repoObserver->notifyError("The provided file index does not exist.");
                return true;
            }


            // get repo browser
            RepositoryInfo repo = fileCache[index].first;
            IRepoBrowser *browser = RepoBrowserFactory::getRepoBrowser(repo); // may return null
            if (browser) {
                // download file
                repoObserver->notifyGetFileComplete(browser->getFile(fileCache[index].second), repo);
                return true;
            }

            repoObserver->notifyError("Error, something went wrong requesting file at '" + repo.GetName() + "'.");
            return true;

        }

        return false;
    }

    static StringVector getHelp() {
        StringVector v;

        return v;
    }

    void setObserver(IRepoObserver *repoObserver) {
        this->repoObserver = repoObserver;
    }

private:
    IRepoObserver *repoObserver;
    int globalFileIndex;
    FileCache fileCache;

    StringVector pushIndex(RepositoryInfo repo, StringVector files) {
        StringVector r;

        BOOST_FOREACH(std::string file, files) {
            r.push_back(boost::lexical_cast<std::string>(++globalFileIndex) + "  ");
            r.push_back(file);

            // update cache          
            RepoFile rf(repo, file);
            fileCache.insert(std::pair<int, RepoFile>(globalFileIndex, rf));
        }
        return r;
    }

    template<typename T>
    T pop_front(std::vector<T>& vec) {
        T t = vec.front();
        assert(!vec.empty());
        vec.erase(vec.begin());
        return t;
    }
};


#endif	/* REPOCLIENT_H */

