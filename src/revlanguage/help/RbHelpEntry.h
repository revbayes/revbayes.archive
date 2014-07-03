/* 
 * File:   RbHelpEntry.h
 * Author: johadunf
 *
 * Created on May 8, 2014, 6:12 PM
 */

#ifndef RBHELPENTRY_H
#define	RBHELPENTRY_H

#include <string>
#include <vector>

///////////////////////////// POCOS representing Rev function help entry /////////////////////////////////////

class ReferenceHelpEntry {
public:

    void SetDoi(std::string doi) {
        this->doi = doi;
    }

    std::string GetDoi() const {
        return doi;
    }

    void SetUrl(std::string url) {
        this->url = url;
    }

    std::string GetUrl() const {
        return url;
    }

    void SetCitation(std::string citation) {
        this->citation = citation;
    }

    std::string GetCitation() const {
        return citation;
    }

private:
    std::string citation;
    std::string url;
    std::string doi;
};

class ArgumentHelpEntry {
public:

    void SetDefaultValue(std::string defaultValue) {
        this->defaultValue = defaultValue;
    }

    std::string GetDefaultValue() const {
        return defaultValue;
    }

    void SetOptions(std::vector<std::string> options) {
        this->options = options;
    }

    std::vector<std::string> GetOptions() const {
        return options;
    }

    void SetValueType(std::string valueType) {
        this->valueType = valueType;
    }

    std::string GetValueType() const {
        return valueType;
    }

    void SetArgumentType(std::string argumentType) {
        this->argumentType = argumentType;
    }

    std::string GetArgumentType() const {
        return argumentType;
    }

    void SetDescription(std::string description) {
        this->description = description;
    }

    std::string GetDescription() const {
        return description;
    }

    void SetLabel(std::string label) {
        this->label = label;
    }

    std::string GetLabel() const {
        return label;
    }


private:

    std::string label;
    std::string description;
    std::string argumentType;
    std::string valueType;
    std::vector<std::string> options;
    std::string defaultValue;
};

class FunctionHelpEntry {
public:

    void SetSeeAlso(std::vector<std::string> seeAlso) {
        this->seeAlso = seeAlso;
    }

    std::vector<std::string> GetSeeAlso() const {
        return seeAlso;
    }

    void SetAuthor(std::string author) {
        this->author = author;
    }

    std::string GetAuthor() const {
        return author;
    }

    void SetExample(std::string example) {
        this->example = example;
    }

    std::string GetExample() const {
        return example;
    }

    void SetDetails(std::vector<std::string> details) {
        this->details = details;
    }

    std::vector<std::string> GetDetails() const {
        return details;
    }

    void SetReturnValue(std::string returnValue) {
        this->returnValue = returnValue;
    }

    std::string GetReturnValue() const {
        return returnValue;
    }

    void SetArguments(std::vector<ArgumentHelpEntry> arguments) {
        this->arguments = arguments;
    }

    std::vector<ArgumentHelpEntry> GetArguments() const {
        return arguments;
    }

    void SetUsage(std::string usage) {
        this->usage = usage;
    }

    std::string GetUsage() const {
        return usage;
    }

    void SetDescription(std::vector<std::string> description) {
        this->description = description;
    }

    std::vector<std::string> GetDescription() const {
        return description;
    }

    void SetTitle(std::string title) {
        this->title = title;
    }

    std::string GetTitle() const {
        return title;
    }

    void SetName(std::string name) {
        this->name = name;
    }

    std::string GetName() const {
        return name;
    }

    void SetReferences(std::vector<ReferenceHelpEntry> references) {
        this->references = references;
    }

    std::vector<ReferenceHelpEntry> GetReferences() const {
        return references;
    }

private:
    std::string name;
    std::string title;
    std::vector<std::string> description;
    std::string usage;
    std::vector<ArgumentHelpEntry> arguments;
    std::string returnValue;
    std::vector<std::string> details;
    std::string example;
    std::vector<ReferenceHelpEntry> references;
    std::string author;
    std::vector<std::string> seeAlso;


};

///////////////////////////// POCOS representing Rev type help entry /////////////////////////////////////

class MethodMemberHelpEntry {
public:

    void SetArguments(std::vector<ArgumentHelpEntry> arguments) {
        this->arguments = arguments;
    }

    std::vector<ArgumentHelpEntry> GetArguments() const {
        return arguments;
    }

    void SetMethodType(std::string methodType) {
        this->methodType = methodType;
    }

    std::string GetMethodType() const {
        return methodType;
    }

    void SetUsage(std::string usage) {
        this->usage = usage;
    }

    std::string GetUsage() const {
        return usage;
    }

    void SetDescription(std::string description) {
        this->description = description;
    }

    std::string GetDescription() const {
        return description;
    }

    void SetMethodName(std::string methodName) {
        this->methodName = methodName;
    }

    std::string GetMethodName() const {
        return methodName;
    }

    void SetReturnValue(std::string returnValue) {
        this->returnValue = returnValue;
    }

    std::string GetReturnValue() const {
        return returnValue;
    }
private:
    std::string methodName;
    std::string description;
    std::string usage;
    std::string methodType;
    std::vector<ArgumentHelpEntry> arguments;
    std::string returnValue;
};

class TypeHelpEntry {
public:

    void SetCitation(std::vector<std::string> citation) {
        this->citation = citation;
    }

    std::vector<std::string> GetCitation() const {
        return citation;
    }

    void SetAuthor(std::string author) {
        this->author = author;
    }

    std::string GetAuthor() const {
        return author;
    }

    void SetDetails(std::vector<std::string> details) {
        this->details = details;
    }

    std::vector<std::string> GetDetails() const {
        return details;
    }

    void SetMethodMembers(std::vector<MethodMemberHelpEntry> methodMembers) {
        this->methodMembers = methodMembers;
    }

    std::vector<MethodMemberHelpEntry> GetMethodMembers() const {
        return methodMembers;
    }

    void SetUsage(std::string usage) {
        this->usage = usage;
    }

    std::string GetUsage() const {
        return usage;
    }

    void SetDescription(std::vector<std::string> description) {
        this->description = description;
    }

    std::vector<std::string> GetDescription() const {
        return description;
    }

    void SetTitle(std::string title) {
        this->title = title;
    }

    std::string GetTitle() const {
        return title;
    }

    void SetName(std::string name) {
        this->name = name;
    }

    std::string GetName() const {
        return name;
    }
private:
    std::string name;
    std::string title;
    std::vector<std::string> description;
    std::string usage;
    std::vector<MethodMemberHelpEntry> methodMembers;
    std::vector<std::string> details;
    std::string author;
    std::vector<std::string> citation;

};


///////////////////////////// POCO representing a complete Rev help entry /////////////////////////////////////////

class RbHelpEntry {
public:

    void SetTypeHelpEntry(TypeHelpEntry typeHelpEntry) {
        this->typeHelpEntry = typeHelpEntry;
    }

    TypeHelpEntry GetTypeHelpEntry() const {
        return typeHelpEntry;
    }

    void SetFunctionHelpEntry(FunctionHelpEntry functionHelpEntry) {
        this->functionHelpEntry = functionHelpEntry;
    }

    FunctionHelpEntry GetFunctionHelpEntry() const {
        return functionHelpEntry;
    }

private:
    FunctionHelpEntry functionHelpEntry;
    TypeHelpEntry typeHelpEntry;
};

#endif	/* RBHELPENTRY_H */

