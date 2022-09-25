//Additional Features:
//	1) Kill program after output of a file or separate command
//		1.1) possibly add threading
//	2) Kill Sleep Thread when desired output

#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>

#define PressEnter std::cout << "\nPress \"Enter\" to Continue "; std::cin.get()
#define print(x); std::cout << x << std::endl
#define String std::string

bool KillProgram = false;

void Help();
bool IsIn(String Str, String Sub);
void Sleep(int time);
void Timer(int time);
void CodeExecute(String command, int Clear, int time);
String GetExecute(String command, int Clear, int time);
void Execution(String command, int Clear, int time, String GetOutput);
void Run(String command, bool Clear, int time, int repeat, bool Show, bool Wait, String GetOutput);

void Help()
{
	String Version = "0.1.5";
	print("Author: Dan (DJ) Coffman");
	print("Program: \"rp\"");
	print("Version: "<< Version);
	print("Purpose: run a command/script on repeat");
	print("Usage: rp <args>");
	print("\t-c <command|script file>\t: command or script to execute");
	print("\t--comand <command|script file>\t: command or script to execute");
	print("\t-s <sleep in seconds>\t\t: wait *seconds* before next execution");
	print("\t--sleep <sleep in seconds>\t: wait *seconds* before next execution");
	print("\t-r <number to repeat>\t\t: repeat (n) times");
	print("\t--repeat <number to repeat>\t: repeat (n) times");
	print("\t-t <kill after time>\t\t: kill command after *seconds* of time");
	print("\t--timer <kill after time>\t: kill command after *seconds* of time");
	print("\t--show\t\t\t\t: show runtime info");
	print("\t--wait\t\t\t\t: wait for next execution from user");
	print("\t--prompt\t\t\t: prompt execution from user");
	print("\t--find <look for>\t\t: continue running program until a given output");
	print("\t--for <look for>\t\t: continue running program until a given output");
	print("\t-clr\t\t\t\t: clear output from last command");
	print("\t--clear\t\t\t\t: clear output from last command");
	print("\n[NOTE] \"ctrl+c\" to kill program");
}

void Sleep(int time)
{
	unsigned long sleep = 1000;

	if (time == 0)
	{
		sleep = 100;
	}
	else
	{
		sleep = time * 1000;
	}
	usleep(sleep * 1000);
}

void Timer(int time)
{
	Sleep(time);
	KillProgram = true;
}

//Check if sub-string is in string
bool IsIn(String Str, String Sub)
{
	bool found = false;
	if (Str.find(Sub) != String::npos)
	{
		found = true;
	}
	return found;
}

String shell(String command)
{
	char buffer[128];
	String result = "";

	// Open pipe to file
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe)
	{
		return "popen failed!";
	}

	// read till end of process:
	while (!feof(pipe))
	{
		// use buffer to read and add to result
		if (fgets(buffer, 128, pipe) != NULL)
		{
			result += buffer;
		}
	}

	pclose(pipe);
	return result;
}

void CodeExecute(String command, int Clear, int time)
{
	system(command.c_str());
	Sleep(time);
	if (Clear == true)
	{
		system("clear");
	}
}

String GetExecute(String command, int Clear, int time)
{
	String TheOut = "";
	TheOut = shell(command.c_str());
	Sleep(time);
	if (Clear == true)
	{
		system("clear");
	}
	return TheOut;
}

void Execution(String command, int Clear, int time, String GetOutput)
{
	String TheOutput = "";
	bool IsFound = false;
	//execute and show output of command/script
	if (GetOutput == "")
	{
		CodeExecute(command, Clear, time);
	}
	//execute, show, and evaluate output of command/script
	else
	{
		//execute and save output
		TheOutput = GetExecute(command, Clear, time);
		//print output of command
		print(TheOutput);
		//check if output contains a given element
		IsFound = IsIn(TheOutput, GetOutput);
		if (IsFound == true)
		{
			//kill program
			KillProgram = true;
		}
	}
}

void Run(String command, bool Clear, int time, int repeat, bool Show, bool Wait, String GetOutput)
{
	//loop forever
	if (repeat == 0)
	{
		int lp = 1;
		while (true)
		{
			//print output of runtime
			if (Show == true)
			{
				if (time != 0)
				{
					print("\t>>>["<< lp <<"] (" << time << " sec)");
				}
				else
				{
					print("\t>>>["<< lp <<"]");
				}
			}

			Execution(command, Clear, time, GetOutput);

			//just kill program
			if (KillProgram == true)
			{
				break;
			}

			//wait for user
			if (Wait == true)
			{
				PressEnter;
			}
			//keep track of runtime
			if (Show == true)
			{
				lp++;
			}
		}
	}
	//loop for a given number of times
	else
	{
		for (int lp = 0; lp != repeat; lp++)
		{
			//print output of runtime
			if (Show == true)
			{
				if (time != 0)
				{
					print("\t>>>[" << (lp+1) << " of " << repeat << "] (" << time << " sec)");
				}
				else
				{
					print("\t>>>[" << (lp+1) << " of " << repeat << "]");
				}
			}

			Execution(command, Clear, time, GetOutput);

			//just kill program
			if (KillProgram == true)
			{
				break;
			}

			//wait for user
			if (Wait == true)
			{
				PressEnter;
			}
		}
		//show execution
		if (Show == true)
		{
			//show end of execution
			print("\nexecution complete");
		}
	}
}

//handle terminal argc
   //argc = count of argc given
   //argv = values of argc given
int main(int argc, char* argv[])
{
	String Command = "";
	String LookForOutput = "";
	int SleepTime = 0;
	int NumberOfRepeat = 0;
	int TheTimer = 0;
	bool ClearScreen = false;
	bool HelpScreen = false;
	bool ShowInfo = false;
	bool IsNotOk = false;
	bool WaitForUser = false;

	//Eval user input
	if (argc > 1)
	{
		String now;
		String value;
		int next = 0;
		//loop through user arguments
		for (int i = 1; i < argc; i++)
		{
			//get current argument
			now = String(argv[i]);

			//Get command and output of command
			if ((now == "-c") || (now == "--comand") || (now == "--for") || (now == "--find"))
			{
				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
					//Get command
					if ((now == "-c") || (now == "--comand"))
					{
						Command = value;
					}
					//Look for output
					else if ((now == "--for") || (now == "--find"))
					{
						LookForOutput = value;
					}
					i++;
				}
				else
				{
					//Program will fail...show help screen
					HelpScreen = true;
					break;
				}
			}

			//Get sleep in seconds
			else if ((now == "-s") || (now == "--sleep") || (now == "-r") || (now == "--repeat") || (now == "-t") || (now == "--timer"))
			{
				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
					IsNotOk = IsIn(value,"-");
					if (IsNotOk == false)
					{
						//Get sleep in seconds
						if ((now == "-s") || (now == "--sleep"))
						{
							SleepTime = stoi(value);
						}
						//Enable repeat command number of times
						else if ((now == "-r") || (now == "--repeat"))
						{
							NumberOfRepeat = stoi(value);
						}
						//Enable kill after time
						else if ((now == "-t") || (now == "--timer"))
						{
							TheTimer = stoi(value);
						}
						i++;
					}
					else
					{
						//Program will fail...show help screen
						HelpScreen = true;
						break;
					}
				}
				else
				{
					//Program will fail...show help screen
					HelpScreen = true;
					break;
				}
			}

			//show Cound of command
			else if (now == "--show")
			{
				ShowInfo = true;
			}

			//Enable clear screen
			else if ((now == "-clr") || (now == "--clear"))
			{
				ClearScreen = true;
			}

			//Enable clear screen
			else if ((now == "--wait") || (now == "--prompt"))
			{
				WaitForUser = true;
			}

			//Show help page
			else if ((now == "-h") || (now == "--help"))
			{
				HelpScreen = true;
			}
		}

		//Show help page
		if ((Command == "") || (HelpScreen == true))
		{
			Help();
		}
		//run repeated command
		else
		{
			if (TheTimer != 0)
			{
				//spawn new thread
				std::thread TimeRun(Timer,TheTimer);
				std::thread RunCommand(Run,Command, ClearScreen, SleepTime, NumberOfRepeat, ShowInfo, WaitForUser, LookForOutput);

				//synchronize threads
				TimeRun.join();
				RunCommand.join();
			}
			else
			{
				Run(Command, ClearScreen, SleepTime, NumberOfRepeat, ShowInfo, WaitForUser, LookForOutput);
			}
		}
	}
	//nothing given by user
	else
	{
		Help();
	}

	return 0;
}
