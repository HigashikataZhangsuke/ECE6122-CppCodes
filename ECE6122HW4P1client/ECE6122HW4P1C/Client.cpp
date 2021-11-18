/*
Author:<Yinzhe Zhang>
Class:ECE6122
Last Date Modified:<06/11/2021>

Description:

This is the TCP client which can handle input and send them to server.

*/
#include <sfml/Network.hpp>
#include <iostream>
#include <string>
#include <windows.h>

bool checkIfAllNum(std::string str)
/*
This is the function used for check if input portnumber is all numbers.
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

bool checkIfNumeric(std::string str)
/*
This is the function used for check if input IP Address is all numbers.
Input:string
Output: bool flag
*/
{
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if ((tmp >= 48 && tmp <= 57) || (tmp == 46))
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
Input:command line arguments
*/
{
    //Deal with our input here
    std::string localhost = "127.0.0.1";
    std::string localhostT = "localhost";
    //Arg numbers
    if (argc == 1)
    {
        std::cout << "Invalid command line argument detected: missing ipaddress and portnumber!";
        return 0;
    }
    else if (argc == 2)
    {
        //Determine which one is the missing argument
        if (checkIfAllNum(std::string(argv[1])) == true)
        {
            // All number, this is portnumber
            std::cout << "Invalid command line argument detected: missing ipaddress!";
            return 0;
        }
        else
        {
            std::cout << "Invalid command line argument detected: missing portnumber!";
            return 0;
        }
    }
    else if (argc > 3)
    {
        std::cout << "Invalid command line argument detected: Received more arguments than expected!";
        return 0;
    }
    //Now just consider if they are valid
    std::string IpAdd = argv[1];
    std::string portstring = argv[2];
    //IP has problem,neither correct number nor text input!
    if (IpAdd != localhostT)
    {
        if (((IpAdd == sf::IpAddress::None) || (checkIfNumeric(IpAdd) == false)))
        {
            std::cout << "Invalid command line argument detected: " << IpAdd << std::endl << "Please check your values and press any key to end the program!";
            return 0;
        }
    }
    //IP no problem but port have problem
    //If we input a portnumber contains letters
    if (checkIfAllNum(portstring) == false)
    {
        std::cout << "Invalid command line argument detected: " << portstring << std::endl << "Please check your values and press any key to end the program!";
        return 0;
    }
    //Then just check range
    int portnumber = std::stoi(portstring);
    if (portnumber < 61000 || portnumber>65535)
    {
        std::cout << "Invalid command line argument detected: " << portnumber << std::endl << "Please check your values and press any key to end the program!";
        return 0;
    }


    // Ask for the server address
    sf::IpAddress server = IpAdd;

    // Create a socket for communicating with the server
    sf::TcpSocket socket;
    //Check connection
    if (socket.connect(IpAdd, portnumber) != sf::Socket::Done)
    {
        std::cout << "Failed to connect to the server at " << IpAdd << " on " << portnumber << " . " << std::endl << "Please check your values and press any key to end program!";
        return 0;
    }
    //While not quit
    while (true)
    {
        // Try to Connect to the server
        //This means we do not find the server
        //Then we connected, should prompt messages
        std::cout << "Please enter a message" << std::endl;
        //Initialize
        std::string Msg;
        std::string characters;
        sf::Packet MsgPack;
        std::getline(std::cin, characters);
        //check if length of msg is bigger than 1000, if bigger than 1000,just take first 1000 characters, and then store in sf::Packet
        if (characters.length() > 1000)
        {
            Msg = characters.substr(0, 1000);
            MsgPack << Msg;
        }
        else
        {
            MsgPack << characters;
        }
        //Send msg, and check connection for each loop
        if (socket.send(MsgPack) != sf::Socket::Done)
        {
            std::cout << "Failed to connect to the server at " << IpAdd << " on " << portnumber << " . " << std::endl << "Please check your values and press any key to end program!";
            return 0;
        }
    }
}