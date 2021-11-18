//如何表示line？如何表示Line的方向？
void CalculateIntersectpoint()
{

}

void CalculateLinerepresentation(std::array<double, 2>& point1, std::array<double, 2>& point2, std::array<double, 4>& line)//Get reflect line!!!Need get current line first, the judge which line will meet.
{
    line[0] = point1[0];
    line[1] = point1[1];// Get the start point
    line[3] = point2[0] - point1[0];
    line[4] = point2[1] - point1[1];//Get the vector
}

int getwhichside(const std::array<double, 2>& point, const std::array<double, 4>& line)
{


}

void CalculateLineEqualtion(std::array<double, 4>& line1, std::array<double, 4>& normline2)
{
    std::array<double, 2> in = { line1[2],line1[3] };
    std::array<double, 2> nor = { normline2[2],normline2[3] };
    line1[2] = in[0] - 2.0 * (in[0] * nor[0] + in[1] * nor[1]) * nor[0];
    line1[3] = in[1] - 2.0 * (in[0] * nor[0] + in[1] * nor[1]) * nor[1];
}
bool isOut(std::array<double, 2> point)
{
    bool flag = false;
    if (point[1] <= 0.01)
    {
        flag = true;
    }
    return true;
}

bool compareStrings(std::string str1, std::string str2)
/*
This is a simple function used to compare two strings(all formed by numbers).
Input: Two strings.
Output: Bool value
*/
{
    bool flag = false;
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if (len1 > len2)
    {
        flag = true; // True means overflow
    }
    else if (len1 == len2)
    {
        flag = (str1 >= str2);
    }
    else
    {
        flag = false;
    }
    return flag;
}
//边界条件还得改，注意要(10,10)的才算OK，否则直接G。
bool isValid(int argnum, std::string inputArgument)
/*
This is the function used to judge whether current input is a vaild number.
Input: An int number and a string.
Output: Bool value
*/
{
    const char* string = inputArgument.data(); //Transfer data type.
    bool flagOfInputNumber = true; //More than one parameter will be filtered.
    bool flagOfInputArgument = (std::all_of(string, string + strlen(string), [](unsigned char c) { return ::isdigit(c); })); //characters and minus number will be filtered.
    bool flagOfInputRange = true;
    if (argnum != 2) //One for filename and one for parameter.
    {
        flagOfInputNumber = false;
    }
    if (compareStrings(string, "18446744073709551615") == true) //Check if the input overflow,18446744073709551615 is the threshold of PACE-ICE unsigned long numbers .
    {
        flagOfInputRange = false;
    }
    bool flag = (flagOfInputArgument && flagOfInputNumber && flagOfInputRange);
    return flag;
}

void writeFile(bool ifValid, int reflectnum)
/*
This is a function to write output info to the file we want.
Input: One bool values and a string.
*/
{
    std::ofstream myfile;
    myfile.open("output3.txt"); //Open the file we will write.
    if (ifValid == true) //Under this condition, the input is vaild.
    {
        myfile << reflectnum;

    }
    else //Or we need to report something wrong happened.
    {
        myfile << "Invalid Input";
    }
    myfile.close(); //Close the file.
}

int main(int argc, char* argv[])
/*
This is the main function, used to call GetPrimeFactors as well as other functions.
Input: A command line parameter.
*/
{
    if (argc != 2)
    {
        writeFile(false, 0);
    }
    else
    {
        if (isValid(argc, argv[1]) == true) //Input must be valid, and only one input is accepted.
        {
            std::array<double, 2> original = { 0.0,0.0 };
            std::array<double, 2> Intersec = { std::atof(argv[1]),sqrt(300) };
            std::array<double, 4> line = { 1,1,1,1 };
            std::array<double, 4> lineAB = { -5.0,sqrt(300),10.0,0.0 };
            std::array<double, 4> lineAC = { -5.0,sqrt(300),5.0,-sqrt(300) };
            std::array<double, 4> lineBC = { 5.0,sqrt(300),-5.0,-sqrt(300) };
            std::array<double, 4> lineABN = { 0.0,sqrt(300),0.0,-1.0 };
            std::array<double, 4> lineACN = { -2.5,sqrt(300) / 2,sqrt(3) / 4,0.5 };
            std::array<double, 4> lineBCN = { 2.5,sqrt(300),-sqrt(3) / 4,0.5 };
            CalculateLinerepresentation(original, Intersec, line);//Get initial Line
            double reflectcount = 1; //At least need one reflect
            bool out = false;
            int whichreflect = 0;//Start with AB side
            while (out == false)
            {
                switch (whichreflect)
                {
                case(0):
                    CalculateLineEqualtion(line, lineABN);
                    break;
                case(1):
                    CalculateLineEqualtion(line, lineACN);
                    break;
                case(2):
                    CalculateLineEqualtion(line, lineBCN);
                    break;

                }
                whichreflect = getwhichside(Intersec, line);//Next time to which side?
                CalculateIntersectpoint();
                out = isOut(Intersec);
                if (out == false)
                {
                    reflectcount += 1;
                }
            }
        }
        else
        {
            writeFile(false, 0);
        }
    }
    return 0;
}

