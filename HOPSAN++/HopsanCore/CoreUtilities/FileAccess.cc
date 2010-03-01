//!
//! @file   FileAccess.cc
//! @author Robert Braun <robert.braun@liu.se>
//! @date   2010-02-03
//!
//! @brief Contains the file access functions
//!
//$Id$

#include <iostream>
#include <cassert>
#include "FileAccess.h"
#include "../Component.h"
#include "../HopsanCore.h"


FileAccess::FileAccess()
{
    //Nothing
}

FileAccess::FileAccess(string filename)
{
    mFilename = filename;
}

void FileAccess::setFilename(string filename)
{
    mFilename = filename;
}

ComponentSystem* FileAccess::loadModel(HopsanEssentials* pHopsan, double *startTime, double *stopTime, string *plotComponent, string *plotPort)
{
        //Read from file
    cout << "Trying to open model: " << mFilename.c_str() << endl;
    ifstream modelFile (mFilename.c_str());
    if(!modelFile.is_open())
    {
        cout << "Model file does not exist!" << endl;
        assert(false);
        //TODO: Cast an exception
    }

        //Necessary declarations
    ComponentSystem* pMainModel = new ComponentSystem("mainModel");
    typedef map<string, Component*> mapComponentType;
    typedef map<string, ComponentSystem*> mapSystemType;
	mapComponentType componentMap;
	mapSystemType componentSystemMap;
    string inputLine;
    string inputWord;

    while (! modelFile.eof() )
    {
            //Read the line
        getline(modelFile,inputLine);                                   //Read a line
        stringstream inputStream(inputLine);

            //Extract first word unless stream is empty
        if ( inputStream >> inputWord )
        {
            cout << inputWord << endl;

            //----------- Create New SubSystem -----------//

            if ( inputWord == "SUBSYSTEM" )
            {
                ComponentSystem *tempComponentSystem = new ComponentSystem();
                inputStream >> inputWord;
                tempComponentSystem->setName(inputWord);
                componentMap.insert(pair<string, Component*>(inputWord, &*tempComponentSystem));
                componentSystemMap.insert(pair<string, ComponentSystem*>(inputWord, &*tempComponentSystem));
                inputStream >> inputWord;
                tempComponentSystem->setTypeCQS(inputWord);
                if ( inputStream >> inputWord )
                {
                    componentSystemMap.find(inputWord)->second->addComponent(tempComponentSystem);      //Subsystem belongs to other subsystem
                }
                else
                {
                    pMainModel->addComponent(tempComponentSystem);                 //Subsystem belongs to main system
                }
            }

            //----------- Add Port To SubSystem -----------//

            else if ( inputWord == "SYSTEMPORT")
            {
                string subSystemName, portName;
                //Component* getComponent(string name);
                inputStream >> subSystemName;
                inputStream >> portName;
                componentSystemMap.find(subSystemName)->second->addSystemPort(portName);
            }

            //----------- Create New Component -----------//

            else if ( inputWord == "COMPONENT" )
            {
                inputStream >> inputWord;
                Component *tempComponent = pHopsan->CreateComponent(inputWord);
                inputStream >> inputWord;
                tempComponent->setName(inputWord);
                componentMap.insert(pair<string, Component*>(inputWord, &*tempComponent));
                if ( inputStream >> inputWord )
                {
                    componentSystemMap.find(inputWord)->second->addComponent(tempComponent);      //Componenet belongs to subsystem
                }
                else
                {
                    pMainModel->addComponent(tempComponent);                 //Component belongs to main system
                }
            }

            //----------- Connect Components -----------//

            else if ( inputWord == "CONNECT" )
            {
                string firstComponent, firstPort, secondComponent, secondPort;
                inputStream >> firstComponent;
                inputStream >> firstPort;
                inputStream >> secondComponent;
                inputStream >> secondPort;

                if ( componentMap.count(firstComponent) > 0 && componentMap.count(secondComponent) > 0 )        //Connecting two components
                {
                    componentMap.find(firstComponent)->second->getSystemParent().connect(*componentMap.find(firstComponent)->second, firstPort, *componentMap.find(secondComponent)->second, secondPort);
                }
                else if ( componentMap.count(firstComponent) > 0 && componentSystemMap.count(secondComponent) > 0 )     //Connecting component with subsystem
                {
                    componentMap.find(firstComponent)->second->getSystemParent().connect(*componentMap.find(firstComponent)->second, firstPort, *componentSystemMap.find(secondComponent)->second, secondPort);
                }
                else if ( componentSystemMap.count(firstComponent) > 0 && componentMap.count(secondComponent) > 0 )     //Connecting subsystem with component
                {
                   componentMap.find(secondComponent)->second->getSystemParent().connect(*componentSystemMap.find(firstComponent)->second, firstPort, *componentMap.find(secondComponent)->second, secondPort);
                }
                else  //Connecting subsystem with subsystem
                {
                     componentMap.find(firstComponent)->second->getSystemParent().connect(*componentSystemMap.find(firstComponent)->second, firstPort, *componentSystemMap.find(secondComponent)->second, secondPort);
                }
            }
                //Execute commands
            //----------- Set Parameter Value -----------//

            else if ( inputWord == "SET" )
            {
                string componentName, parameterName;
                double parameterValue;
                inputStream >> componentName;
                inputStream >> parameterName;
                inputStream >> parameterValue;
                componentMap.find(componentName)->second->setParameter(parameterName, parameterValue);
            }

            //----------- Set Simulation Parameters -----------//

            else if ( inputWord == "SIMULATE" )
            {
                double temp;
                inputStream >> temp;
                *startTime = temp;
                inputStream >> temp;
                *stopTime = temp;
            }

            //----------- Select Plotting Parameters -----------//

            else if ( inputWord == "PLOT" )
            {
                inputStream >> *plotComponent;
                inputStream >> *plotPort;
            }

            //----------- Unrecognized Command -----------//

            else
            {
                cout << "Unidentified command in model file ignored.\n";
            }
        }
        else
        {
            //cout << "Ignoring empty line.\n";
        }

    }
    modelFile.close();

    return pMainModel;
}

ComponentSystem* FileAccess::loadModel(HopsanEssentials* pHopsan, string filename, double *startTime, double *stopTime, string *plotComponent, string *plotPort)
{
    setFilename(filename);
    return loadModel(pHopsan, &*startTime, &*stopTime, &*plotComponent, &*plotPort);
}

void FileAccess::saveModel(string fileName, ComponentSystem* pMotherOfAllModels, double startTime, double stopTime, string plotComponent, string plotPort)
{
    ofstream modelFile(fileName.c_str());
    saveComponentSystem(modelFile, pMotherOfAllModels, "");
    modelFile << "SIMULATE " << startTime << " " << stopTime << "\n";
    modelFile << "PLOT " << plotComponent << " " << plotPort << "\n";
    modelFile.close();
    return;
}


void FileAccess::saveComponentSystem(ofstream& modelFile, ComponentSystem* pMotherModel, string motherSystemName)
{
    vector<string> mainComponentList = pMotherModel->getSubComponentNames();
    vector<string>::iterator it;
    map<Port*, string> portList;

    for(it = mainComponentList.begin(); it!=mainComponentList.end(); ++it)
    {
        if(pMotherModel->getSubComponent(*it)->isComponentSystem())
        {
            modelFile << "SUBSYSTEM " << " " << *it << " " << pMotherModel->getSubComponentSystem(*it)->getTypeCQS() << "\n";
            vector<Port*> systemPorts = pMotherModel->getSubComponentSystem(*it)->getPortPtrVector();
            cout << "Subsystem has " << systemPorts.size() << " ports.\n";
            vector<Port*>::iterator itp;
            for (itp=systemPorts.begin(); itp!=systemPorts.end(); ++itp)
            {
                modelFile << "SYSTEMPORT " << *it << " " << (*itp)->getPortName() << endl;
            }

            saveComponentSystem(modelFile, pMotherModel->getSubComponentSystem(*it), motherSystemName + " " + *it);
        }
        else
        {
                //Write the create component line in the file
            modelFile << "COMPONENT " << pMotherModel->getSubComponent(*it)->getTypeName() << " " << *it << motherSystemName << endl;
        }

        map<string, double> componentParameterMap = pMotherModel->getSubComponent(*it)->getParameterMap();

        map<string, double>::iterator itc;
        for(itc = componentParameterMap.begin(); itc!=componentParameterMap.end(); ++itc)
        {
            modelFile << "SET " << *it << " " << itc->first << " " << itc->second << "\n";
        }

            //Store all ports in a map, together with the name of the component they belong to (for use below)
        vector <Port*> portPtrsVector = pMotherModel->getSubComponent(*it)->getPortPtrVector();
        vector <Port*>::iterator itp;
        for (itp=portPtrsVector.begin(); itp!=portPtrsVector.end(); ++itp)
        {
            portList.insert(pair<Port*,string>(*itp, *it));
        }
        portPtrsVector = pMotherModel->getPortPtrVector();
        for (itp=portPtrsVector.begin(); itp!=portPtrsVector.end(); ++itp)
        {
            portList.insert(pair<Port*,string>(*itp, pMotherModel->getName()));
        }
    }

    cout << "Connecting in system " << pMotherModel->getName() << ", portList.size() = " << portList.size() << endl;

        //Iterate through port map and figure out which ports share the same node, and then write the connect lines
    map<Port*, string>::iterator itp;
    for(itp = portList.begin(); itp != portList.end();)
    {
        map<Port*, string>::iterator itp2;
        for(itp2 = portList.begin(); itp2 != portList.end(); ++itp2)
        {
            Node *ptr1 = itp->first->getNodePublic();
            Node *ptr2 = itp2->first->getNodePublic();
            if (ptr1 == ptr2 && itp != itp2)
            {
                modelFile << "CONNECT " << itp->second << " " << itp->first->getPortName() << " " << itp2->second << " " << itp2->first->getPortName() << "\n";
            }
        }
        portList.erase(itp++);          //Increase itp by 1, then remove previous value from map to prevent double connection
    }
}
