#ifndef SOURCEFORGEREPOBROWSER_H
#define	SOURCEFORGEREPOBROWSER_H

#include <vector>
#include <string>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

#include "IRepoBrowser.h"
#include "RepositoryInfo.h"
#include "HttpClient.h"

#include "pugixml.hpp"

typedef std::vector<std::string> StringVector;

class SourceForgeRepoBrowser : public IRepoBrowser {
public:

    SourceForgeRepoBrowser(RepositoryInfo revRepository) {
        this->revRepository = revRepository;
    }

    /**
     * Get a list of .Rev files from the repo
     */
    HttpResponse getIndex() {

        HttpRequest httpRequest;
        
        httpRequest.method = "GET";
        httpRequest.path = revRepository.GetPath();
        httpRequest.host = revRepository.GetServer();
        httpRequest.accept = "*/*"; 
        httpRequest.protocol = revRepository.GetProtocol();

        HttpResponse httpResponse = HttpClient::makeRequest(httpRequest);

        StringVector v;
        boost::split(v, httpResponse.body, boost::is_any_of("\n\r"));
        
        // scrape filenames
        boost::regex e("<span>(.*\\.Rev)</span>", boost::regex::normal);// | boost::regbase::icase);
        httpResponse.data.clear();
        BOOST_FOREACH(std::string s, v) {
            boost::smatch matches;
            if (boost::regex_search(s, matches, e)) {
                httpResponse.data.push_back(matches[1]);
            }            
        }
        
        return httpResponse;
    }
    
    
    HttpResponse getFile(std::string filename) {
        //http://sourceforge.net/p/revbayes/code-git/ci/master/tree/examples/RevLanguageBasicsTutorial.Rev?format=raw
        HttpRequest httpRequest;
        
        httpRequest.method = "GET";
        httpRequest.path = revRepository.GetPath() + filename + "?format=raw";
        httpRequest.host = revRepository.GetServer();
        httpRequest.accept = "*/*"; 
        httpRequest.protocol = revRepository.GetProtocol();

        HttpResponse httpResponse = HttpClient::makeRequest(httpRequest);
        
        // not very clear, but convenient ...
        httpResponse.data.push_back(filename);
        httpResponse.data.push_back(httpResponse.body);
        
        return httpResponse;
    }
    

private:

    RepositoryInfo revRepository;


};


#endif
