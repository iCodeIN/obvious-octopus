#include <iostream>
#include <random>

#include "nlp/ngramlanguagedetector.hpp"
#include "util/filetostring.hpp"

using namespace nlp;
using namespace std;
using namespace util;

void trainModel()
{
    // init model
    NGramLanguageDetector ngld;
    ngld.setOption(IGNORE_CASE);
    ngld.setOption(IGNORE_NON_ALPHANUMERIC);

    // train
    ngld.train(FileToString::readFile("/home/joris/Code/alexandria/corpus/en.txt"), "EN");
    std::cout << "trained for EN" << std::endl;

    ngld.train(FileToString::readFile("/home/joris/Code/alexandria/corpus/fr.txt"), "FR");
    std::cout << "trained for FR" << std::endl;

    ngld.train(FileToString::readFile("/home/joris/Code/alexandria/corpus/nl.txt"), "NL");
    std::cout << "trained for NL" << std::endl;

    ngld.train(FileToString::readFile("/home/joris/Code/alexandria/corpus/es.txt"), "ES");
    std::cout << "trained for ES" << std::endl;

    ngld.train(FileToString::readFile("/home/joris/Code/alexandria/corpus/de.txt"), "DE");
    std::cout << "trained for DE" << std::endl;

    // persist
    ofstream myfile;
    myfile.open ("ngrammodel.xml");
    myfile << *(ngld.toXML().get());
    myfile.close();
}

void testModel()
{
    // init model
    NGramLanguageDetector ngld;
    ngld.setOption(IGNORE_CASE);
    ngld.setOption(IGNORE_NON_ALPHANUMERIC);

    // read to xml
    ifstream ifs;
    ifs.open ("ngrammodel.xml", ifstream::in);

    // read graph
    auto elementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(""));
    ifs >> *(elementPtr.get());

    // set model
    ngld.fromXML(std::move(elementPtr));

    // languages to test
    std::vector<std::string> languages;
    languages.push_back("en");
    languages.push_back("fr");
    languages.push_back("nl");
    languages.push_back("es");
    languages.push_back("de");

    // sizes to test
    std::vector<int> sizes;
    sizes.push_back(16);
    sizes.push_back(32);
    sizes.push_back(64);
    sizes.push_back(128);
    sizes.push_back(256);

    // number of samples
    int nofSamples = 100;

    // random (to select samples)
    std::random_device rnd;

    // Choose a random mean between 1 and 6
    std::default_random_engine rndEngine(rnd());

    for(int i=0; i<languages.size(); i++)
    {
        std::string text = FileToString::readFile("/home/joris/Code/alexandria/corpus/" + languages[i] + ".txt");
        std::uniform_int_distribution<int> uniform_dist(0, text.size() - sizes[sizes.size() - 1]);

        for(int size : sizes)
        {
            std::cout << languages[i] << "\t" << "[" << size << "]" << "\t";
            std::map<std::string, double> classifications;
            classifications["EN"] = 0.0;
            classifications["FR"] = 0.0;
            classifications["NL"] = 0.0;
            classifications["ES"] = 0.0;
            classifications["DE"] = 0.0;

            for(int j=0; j < nofSamples; j++)
            {
                // select text sample
                std::string sample = text.substr(uniform_dist(rndEngine), size);

                // detect language
                std::string maxLang = "";
                double maxLangScore = 0.0;
                for(auto &pair : ngld.detect(sample))
                {
                    if(pair.second > maxLangScore)
                    {
                        maxLangScore = pair.second;
                        maxLang = pair.first;
                    }
                }

                // up the number
                classifications[maxLang]++;
            }
            for(auto &pair : classifications)
            {
                pair.second /= nofSamples;
                std::cout << pair.first << "\t" << pair.second << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void useModel()
{
    // init model
    NGramLanguageDetector ngld;
    ngld.setOption(IGNORE_CASE);
    ngld.setOption(IGNORE_NON_ALPHANUMERIC);

    // read to xml
    ifstream ifs;
    ifs.open ("ngrammodel.xml", ifstream::in);

    // read graph
    auto elementPtr = std::unique_ptr<XML::IElement>(new XML::DefaultElementImpl(""));
    ifs >> *(elementPtr.get());

    // set model
    ngld.fromXML(std::move(elementPtr));

    // use model
    string text = "This is merely a test to determine the quality of the prediction, which hopefully improves as I make the string longer.";
    auto cls = ngld.detect(text);
    for(auto &pair : cls)
    {
        std::cout << pair.first << "\t" << pair.second << std::endl;
    }
}

int main()
{
    trainModel();

    testModel();

    //useModel();

    // return
    return 0;
}
