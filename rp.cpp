#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>

#define PressEnter std::cout << "\nPress \"Enter\" to Continue "; std::cin.get()
#define print(x); std::cout << x << std::endl
#define String std::string

bool KillProgram = false;
String TheName = "";

void Help();
void Sleep(int time);
void Timer(int time);
bool StartsWith(String Str, String Start);
bool IsIn(String Str, String Sub);
String shell(String command);
void CodeExecute(String command, int Clear, int time, bool Wait);
void UnitlExecute(String command);
String GetExecute(String command, int Clear, int time, bool Wait);
void Execution(String command, int Clear, int time, String GetOutput, bool Wait);
void Run(String command, bool Clear, int time, int repeat, bool Show, bool Wait, String GetOutput);

void Help()
{
	String Version = "0.1.13";
	print("Author: Dan (DJ) Coffman");
	print("Program: \"" << TheName << "\"");
	print("Version: "<< Version);
	print("Purpose: run a command/script on repeat");
	print("Usage: " << TheName << " <args>");
	print("{REQUIRED}");
	print("\t-c <command|script>\t\t: command or script to execute");
	print("\t--command <command|script file>\t: command or script to execute");
	print("");
	print("{OPTIONAL}");
	print("\t-s <sleep in seconds>\t\t: wait *seconds* before next execution");
	print("\t--sleep <sleep in seconds>\t: wait *seconds* before next execution");
	print("\t-r <number to repeat>\t\t: repeat (n) times");
	print("\t--repeat <number to repeat>\t: repeat (n) times");
	print("\t-t <kill after time>\t\t: kill command after *seconds* of time");
	print("\t--timer <kill after time>\t: kill command after *seconds* of time");
	print("\t--show\t\t\t\t: show runtime info");
	print("\t--wait\t\t\t\t: wait for next execution from user");
	print("\t--prompt\t\t\t: prompt execution from user");
	print("\t-u <command|script>\t\t: kill program command once other command/script finishes (no output)");
	print("\t--until <command|script>\t: kill program command once other command/script finishes (no output)");
	print("\t\t\t\t\t: overrides: --time, --repeat, and --for");
	print("\t--find <look for>\t\t: continue running program until a given output");
	print("\t--for <look for>\t\t: continue running program until a given output");
	print("\t-clr\t\t\t\t: clear output from last command");
	print("\t--clear\t\t\t\t: clear output from last command");
	print("");
	print("{EXAMPLE}: Run a command forever");
	print("\t$ " << TheName << " -c ls");
	print("");
	print("{EXAMPLE}: Run a command 3 times, sleep for 3 seconds");
	print("\t$ " << TheName << " -c ls -s 3 -r 3");
	print("");
	print("{EXAMPLE}: Run a command until a given output");
	print("\t$ " << TheName << " -c ls -s 3 -r 3 --find \"Documents\"");
	print("");
	print("{EXAMPLE}: Run a command until another command/script is finished running");
	print("\t$ " << TheName << " -c ls -s 3 -r 3 --until \"whoami\"");
	print("");
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

//Check if string begins with substring
bool StartsWith(String Str, String Start)
{
	bool ItDoes = false;
	if (Str.rfind(Start, 0) == 0)
	{
		ItDoes = true;
	}
	return ItDoes;
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

void CodeExecute(String command, int Clear, int time, bool Wait)
{
	system(command.c_str());
	Sleep(time);
	//wait for user
	if (Wait == true)
	{
		PressEnter;
	}
	if ((Clear == true) && (KillProgram != true))
	{
		system("clear");
	}
}

void UnitlExecute(String command)
{
	system(command.c_str());
	KillProgram = true;
}

String GetExecute(String command, int Clear, int time, bool Wait)
{
	String TheOut = "";
	TheOut = shell(command.c_str());
	Sleep(time);
	//wait for user
	if (Wait == true)
	{
		PressEnter;
	}
	if ((Clear == true) && (KillProgram != true))
	{
		system("clear");
	}
	return TheOut;
}

void Execution(String command, int Clear, int time, String GetOutput, bool Wait)
{
	String TheOutput = "";
	bool IsFound = false;
	//execute and show output of command/script
	if (GetOutput == "")
	{
		CodeExecute(command, Clear, time, Wait);
	}
	//execute, show, and evaluate output of command/script
	else
	{
		//execute and save output
		TheOutput = GetExecute(command, Clear, time, Wait);
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

			Execution(command, Clear, time, GetOutput, Wait);

			//just kill program
			if (KillProgram == true)
			{
				break;
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

			if ((lp +1) == repeat)
			{
				KillProgram = true;
			}

			Execution(command, Clear, time, GetOutput, Wait);

			//just kill program
			if (KillProgram == true)
			{
				break;
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
	String OtherCommand = "";
	String LookForOutput = "";
	int SleepTime = 0;
	int NumberOfRepeat = 0;
	int TheTimer = 0;
	bool ClearScreen = false;
	bool HelpScreen = false;
	bool ShowInfo = false;
	bool IsNotOk = false;
	bool WaitForUser = false;

	String out = String(argv[0]);

	//Parsing program name
	std::size_t pos = out.rfind('/');
	TheName = out.substr(pos + 1);
	out = "";

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
			if ((now == "-c") || (now == "--command") || (now == "--for") || (now == "--find") || (now == "-u") || (now == "--until") || (now == "-s") || (now == "--sleep") || (now == "-r") || (now == "--repeat") || (now == "-t") || (now == "--timer"))
			{
				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
					IsNotOk = StartsWith(value,"-");
					if (IsNotOk == false)
					{
						//Get command
						if ((now == "-c") || (now == "--command"))
						{
							Command = value;
						}
						//Look for output
						else if ((now == "--for") || (now == "--find"))
						{
							LookForOutput = value;
						}
						else if ((now == "-u") || (now == "--until"))
						{
							OtherCommand = value;
						}
						//Get sleep in seconds
						else if ((now == "-s") || (now == "--sleep"))
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
			if (OtherCommand != "")
			{
				//reset values
				NumberOfRepeat = 0;
				LookForOutput = "";
				//spawn new thread
				std::thread Until(UnitlExecute,OtherCommand+" > /dev/null");
				std::thread RunCommand(Run,Command, ClearScreen, SleepTime, NumberOfRepeat, ShowInfo, WaitForUser, LookForOutput);

				//synchronize threads
				Until.join();
				RunCommand.join();
			}
			else if (TheTimer != 0)
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
