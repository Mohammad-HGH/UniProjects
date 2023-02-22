// See https://aka.ms/new-console-template for more information
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.Reflection.Metadata.Ecma335;

namespace library2
{
    class Program
    {
        enum MenuItem
        {
            Input_user=1,
            Input_book,
            User_Show,
            Book_Show,
            Book_Subject,
            Edit_User,
            Exit
        };
        static void Main(string[] args)
        {
            int nmember;
            Console.Write("Enter number of member: ");
            nmember=int.Parse(Console.ReadLine());
            string[] MemberName = new string[nmember];
            string[] MemberFamily = new string[nmember];
            int Nbook;
            Console.Write("Enter number of book: ");
            Nbook = int.Parse(Console.ReadLine());
            string[] BookName = new string[Nbook];
            string[] BookTopic = new string[Nbook];

            int choice = 0;
            while (choice!=7)
            {
                Console.WriteLine("  ");
                Console.WriteLine("{1}:{0}",MenuItem.Input_user,(int) MenuItem.Input_user);
                Console.WriteLine("{1}:{0}",MenuItem.Input_book,(int) MenuItem.Input_book);
                Console.WriteLine("{1}:{0}",MenuItem.User_Show,(int) MenuItem.User_Show);
                Console.WriteLine("{1}:{0}",MenuItem.Book_Show,(int) MenuItem.Book_Show);
                Console.WriteLine("{1}:{0}",MenuItem.Book_Subject,(int) MenuItem.Book_Subject);
                Console.WriteLine("{1}:{0}",MenuItem.Edit_User,(int) MenuItem.Edit_User);
                Console.WriteLine("{1}:{0}",MenuItem.Exit,(int) MenuItem.Exit);
                Console.WriteLine("Please enter your choice: ");
                choice = int.Parse(Console.ReadLine());


                switch (choice)
                {
                    case 1:
                        Console.Clear();
                        for (int i = 0; i < nmember; i++) {
                            Console.Write("Enter name of member{0}", i + 1);
                        }
                        break;

                    case 2:
                        Console.Clear();
                        for(int i = 0; i < Nbook; i++)
                        {
                            Console.Write("Enter name of book{0}",i+1);
                            BookName[i] = Console.ReadLine();
                            Console.Write("Enter subject of book{0}", i + 1);
                            BookTopic[i] = Console.ReadLine();
                        }
                        break;
                    case 3:
                        Console.Clear();
                        for (int i=0;i<nmember;i++) {
                            Console.Write("The Member {0} is (1) (2) \n", i + 1, MemberName[i], MemberFamily[i]);
                        }
                        Console.ReadKey();
                        break;
                    case 4:
                        Console.Clear();
                        for (int i=0;i<Nbook;i++) {
                            Console.Write("Name of book{0} is {1} and the subject is {2} \n", i + 1, BookName[i], BookTopic[i]);
                        }
                        Console.ReadKey();
                        break;
                    case 5:
                        Console.Clear();
                        Console.Write("Please enter your subject of book: ");
                        string search = Console.ReadLine();
                        for (int i=0;i<Nbook;i++) {
                            if (search == BookTopic[i])
                            {
                                Console.Write("Name of book is {0} \n", BookName[i]);
                                break;
                            }
                            else Console.Write("NOT FOUND! \n");
                        
                        }
                        Console.ReadKey();
                        break;
                    case 6:
                        Console.Clear();
                        Console.Write("Please enter your number of member: ");
                        int no=int.Parse(Console.ReadLine());
                        Console.Write("Please enter new name: ");
                        MemberName[no - 1] = Console.ReadLine();
                        Console.Write("Please enter new family name: ");
                        MemberFamily[no - 1] = Console.ReadLine();
                        break;
                    case 7:
                        break;
                    default:
                        Console.WriteLine("Your select is invalid. please enter new number: ");
                        break;
                }
            }
        }
    }
}

