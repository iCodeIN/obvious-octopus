#include <iostream>

#include "util/filetostring.hpp"
#include "nlp/ngramlanguagedetector.hpp"

using namespace std;
using namespace nlp;
using namespace util;

void trainModel()
{
    // init model
    NGramLanguageDetector ngld;

    // train

    ngld.train(FileToString::readFile("/home/joris/Documents/corpus/en/en001.txt"), "EN");
    std::cout << "trained for EN" << std::endl;

    ngld.train(FileToString::readFile("/home/joris/Documents/corpus/fr/fr001.txt"), "FR");
    std::cout << "trained for FR" << std::endl;

    /*
     ngld.train(FileToString::readFile("/home/joris/Documents/corpus/nl/nl001.txt"), "NL");
    std::cout << "trained for NL" << std::endl;
    */

    ngld.train(FileToString::readFile("/home/joris/Documents/corpus/es/es001.txt"), "ES");
    std::cout << "trained for ES" << std::endl;

    ngld.train(FileToString::readFile("/home/joris/Documents/corpus/de/de001.txt"), "DE");
    std::cout << "trained for DE" << std::endl;

    // persist
    ofstream myfile;
    myfile.open ("ngrammodel.xml");
    myfile << *(ngld.toXML().get());
    myfile.close();
}

void useModel()
{
    // init model
    NGramLanguageDetector ngld;

    // read to xml
    ifstream ifs;
    ifs.open ("ngrammodel.xml", ifstream::in);

    // read graph
    auto elementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(""));
    ifs >> *(elementPtr.get());

    // set model
    ngld.fromXML(std::move(elementPtr));

    // use model
    string text = "If i could be a superhero, I would be drug-free boy; Freeing the world of the evils of drugs and all of the lives they destroy.";
    text = FileToString::readFile("/home/joris/Documents/corpus/en/en001.txt");
    auto cls = ngld.detect(text);
    for(auto &pair : cls)
    {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
}

int main()
{
    useModel();

    // return
    return 0;
}
