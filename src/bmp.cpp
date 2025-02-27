#include "bmp.h"

bool Bmp::Load(std::string Path)
{
    std::ifstream file(Path, std::ios::binary);

    file.read((char *)&Fhd, sizeof(Fhd));
    if (Fhd.Type != 0x4D42)
    {
        std::cout << "Fail:File does not exist or unsupported file type.";
        return false;
    }

    file.read((char *)&Ihd, sizeof(Ihd));
    if (Ihd.BitCount != 24)
    {
        std::cout << "Fail:The file is not 24-bit and is not supported.";
        return false;
    }
    if (Ihd.InfohderSize != 40)
    {
        std::cout << "Fail:It's not BMP3 and is not supported.";
        return false;
    }
    if (Ihd.Comression != 0)
    {
        std::cout << "Fail:The file had been compression";
        return false;
    }

    mapptr = new (std::nothrow) ub1[LineByteCalculator(Ihd.Width) * Ihd.Height];

    if (mapptr == nullptr)
    {
        std::cout << "Fail:memory run out";
    }

    file.read((char *)mapptr, LineByteCalculator(Ihd.Width) * Ihd.Height);

    std::cout << "successfully load the bmp image!\n";
    std::cout << "It's " << Ihd.Width << "*" << Ihd.Height << "\n";
    return true;
}

bool Bmp::GetMessage()
{
    HideScale = Ihd.Width * 3 * Ihd.Height / 8;
    short chosen;
    std::cout << "There are " << HideScale - 10 << " letters that can be hidden\n";
    std::cout << "1.txt message." << "   2.input message directly." << "  1 or 2 ?";

    std::cin >> chosen;
    if (chosen == 1)
    {
        std::cout << "Now,please enter your message path\n";
        std::string messagepath;
        std::cin.ignore(1, '\n');
        std::getline(std::cin, messagepath);
        messagepath.erase(std::remove(messagepath.begin(), messagepath.end(), '\''), messagepath.end());// remove single quotes from the string
        std::ifstream file(messagepath);
        if (!file)
        {
            std::cout << "Fail: Unable to open the file.";
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        message = buffer.str();
        if (messagepath[messagepath.length() - 1] == 't')
            FileKind = 0;
        else if (messagepath[messagepath.length() - 1] == 'g' && messagepath[messagepath.length() - 2] == 'n')
            FileKind = 1;
        else if (messagepath[messagepath.length() - 1] == 'g' && messagepath[messagepath.length() - 2] == 'p')
            FileKind = 2; // set the file kind
    }// if the user choose to input a file path

    else if (chosen == 2)
    {
        std::cout << "Now,please enter your message directly\n";
        std::cin.ignore(1, '\n');
        std::getline(std::cin, message);
        FileKind = 0;
    }
    else
    {
        std::cout << "Fail:The input is invalid";
        return false;
    }

    if (message.length() > HideScale - 10)
    {
        std::cout << "Fail:信息过长,无法隐藏";
        return false;
    }
    lenthB = std::bitset<64>(message.length()).to_string();
    for (char c : message)
    {
        messageB += ctostr(c);
    }
    // std::cout<<messageB<<"\n";
    return true;
}

bool Bmp::process()
{
    ub1 before;
    for (int i = 1; i <= 3; i++)
    {
        before = mapptr[hidetomap(i)];// set the first three bytes to 0
        // std::cout<<int(before)<<" "<<i<<'\n';
        before &= ub1(254);
        mapptr[hidetomap(i)] = before;
        // std::cout<<int(before)<<"\n\n";
    }

    if (FileKind == 1)
        mapptr[hidetomap(1)] += 1;
    if (FileKind == 2)
        mapptr[hidetomap(2)] += 1;

    for (int i = 4; i <= 64; i++)
    {
        before = mapptr[hidetomap(i)];
        // std::cout<<int(before)<<" "<<i<<'\n';
        before &= ub1(254);
        if (lenthB[i - 1] == '1')
            before += 1;
        mapptr[hidetomap(i)] = before;
        // std::cout<<int(before)<<"\n\n";
    }
    for (int i = 65; i <= 64 + messageB.length(); i++)
    {
        before = mapptr[hidetomap(i)];

        before &= ub1(254);
        if (messageB[i - 1 - 64] == '1')
            before += 1;
        mapptr[hidetomap(i)] = before;
    }
    return true;// if the user choose to input a message directly
}

bool Bmp::Save(std::string Path)
{
    std::ofstream file(Path, std::ios::binary);
    file.write((char *)&Fhd, sizeof(Fhd));
    file.write((char *)&Ihd, sizeof(Ihd));

    file.write((char *)mapptr, LineByteCalculator(Ihd.Width) * Ihd.Height);
    file.close();
    std::cout << "Successfully hide the message!!!\n";
    std::cout << messageB.length() / 8 << "/" << Ihd.Width * 3 * Ihd.Height / 8 - 10;
    delete[] mapptr; // kill the ptr
    return true;
}

bool Bmp::extract()
{
    ub1 before;

    if (mapptr[hidetomap(1)] & ub1(1))
        FileKind = 1;
    else if (mapptr[hidetomap(2)] & ub1(1))
        FileKind = 2;
    else
        FileKind = 0;
    for (int i = 4; i <= 64; i++)
    {
        before = mapptr[hidetomap(i)];
        before &= ub1(1);
        if (before)
            lenthB += '1';
        else
            lenthB += '0';
    }
    ub8 len = std::bitset<64>(lenthB).to_ullong();
    std::cout << "The message's width is " << len << ".";
    if (FileKind == 0)
    {
        std::cout << "\n\n*********************************\n\n";
        for (int i = 65; i <= 64 + len * 8; i++)
        {
            before = mapptr[hidetomap(i)];
            before &= ub1(1);
            if (before)
                messageB += '1';
            else
                messageB += '0';
            // std::cout<<messageB<<"  ";
            if (messageB.length() == 8)
            {
                std::cout << static_cast<char>(std::bitset<8>(messageB).to_ulong());
                messageB.clear();
            }
        }
    }
    else if (FileKind == 1)
    {
        std::cout << "Your file is .png\n";
        std::ofstream file("OutPut.png", std::ios::binary);
        char Byte;
        for (int i = 65; i <= 64 + len * 8; i++)
        {
            before = mapptr[hidetomap(i)];
            before &= ub1(1);
            if (before)
                messageB += '1';
            else
                messageB += '0';
            // std::cout<<messageB<<"  ";
            if (messageB.length() == 8)
            {
                Byte = std::bitset<8>(messageB).to_ulong();
                file.write(&Byte, 1);
                messageB.clear();
            }
        }
        std::cout << "It has been decodered as OutPut.png";
        file.close();
    }
    else if (FileKind == 2)
    {
        std::cout << "Your file is .jpg\n";
        std::ofstream file("OutPut.jpg", std::ios::binary);
        char Byte;
        for (int i = 65; i <= 64 + len * 8; i++)
        {
            before = mapptr[hidetomap(i)];
            before &= ub1(1);
            if (before)
                messageB += '1';
            else
                messageB += '0';
            // std::cout<<messageB<<"  ";
            if (messageB.length() == 8)
            {
                Byte = std::bitset<8>(messageB).to_ulong();
                file.write(&Byte, 1);
                messageB.clear();
            }
        }
        std::cout << "It has been decodered as OutPut.jpg";
        file.close();
    }
    std::cout << "\n\n*********************************";
    return true;
}

ub8 Bmp::LineByteCalculator(ub4 Width)
{
    return int((Width * 3 + 3) / 4) * 4;
}

std::string Bmp::ctostr(char c)
{
    return std::bitset<8>(c).to_string();
}

ub8 Bmp::hidetomap(ub8 a)
{
    short stuffbyte;
    stuffbyte = 4 - (Ihd.Width * 3) % 4;
    if (stuffbyte == 4)
        stuffbyte = 0;
    if (a % (Ihd.Width * 3) != 0)
        return stuffbyte * int(a / (3 * Ihd.Width)) + a;
    else
        return stuffbyte * (int(a / (Ihd.Width * 3)) - 1) + a;
}