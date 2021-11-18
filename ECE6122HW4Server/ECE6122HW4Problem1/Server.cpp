/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<06/11/2021>

Description:

This is the TCP server which can handle multiple clients input.

*/

#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <fstream>

bool checkIfAllNum(std::string str)
/*
This is the function used for check if input is all numbers.
Input:string
Output: bool flag
*/
{
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
/*
This is the main function.
Input:command line argument
*/
{
    //Deal with argument parameter
    if (argc == 1)
    {
        //Do not receive the input
        std::cout << "Invalid command line argument detected: missing portnumber!";
        return 0;
    }
    else if (argc > 2)
    {
        //Receive too many argument
        std::cout << "Invalid command line argument detected: too many arguments";
        return 0;
    }
    //Else we have exactly one input
    //Check if the input is all number
    if (checkIfAllNum(argv[1]) == false)
    {
        std::cout << "Invalid command line argument detected: " << argv[1] << std::endl << "Please check your values and press any key to end the program!";
        return 0;
    }
    //And then check the input range
    int portnumber = std::stoi(argv[1]);
    if (portnumber < 61000 || portnumber > 65535)
    {
        std::cout << "Invalid command line argument detected: " << portnumber << std::endl << "Please check your values and press any key to end the program!";
        return 0;
    }
    //Now use selector to handle multiple clients
    // Create a listener & Listen to the given port for incoming connections
    sf::TcpListener listener;
    listener.listen(portnumber);
    //Create a list and selector
    std::list<sf::TcpSocket*> clients;
    sf::SocketSelector selector;
    //Open the log file for append new line
    std::ofstream myfile;
    //Add listener to selector
    selector.add(listener);
    //If we do not exit, which means close the command window here
    while (true)
    {
        //Make sure selector check the status
        if (selector.wait())
        {
            //Listener find a new client want to connect to server
            if (selector.isReady(listener))
            {
                //create new client
                sf::TcpSocket* client = new sf::TcpSocket;
                //Yes we do connected, add it
                if (listener.accept(*client) == sf::Socket::Done)
                {
                    clients.push_back(client);
                    selector.add(*client);
                    myfile.open("server.log", std::ios::app);
                    myfile << "A new client connected" << '\n';
                    myfile.close();
                    
                    
                }
                //Something happened. the client failed to connect, should delete.
                else
                {
                    delete client;
                }
            }
            //No new client, try to get data from current one
            else
            {
                //iteration
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
                {
                    sf::TcpSocket& client = **it;
                    //client is good
                    if (selector.isReady(client))
                    {
                        sf::Packet pack;
                        //If we received the data pack
                        if (client.receive(pack) == sf::Socket::Done)
                        {
                            //Save in the log file
                            std::string Msg;
                            pack >> Msg;
                            myfile.open("server.log", std::ios::app);
                            myfile << Msg<< '\n';
                            myfile.close();
                            
                           
                        }
                        //Now disconnected
                        else if (client.receive(pack) == sf::Socket::Disconnected)
                        {
                            selector.remove(client);
                            myfile.open("server.log", std::ios::app);
                            myfile << "An existed client disconnected" << '\n';
                            myfile.close();
                        }
                    }
                }
            }
        }
    }
}

