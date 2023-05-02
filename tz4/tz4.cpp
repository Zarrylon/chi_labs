#include <thread>
#include "MessageQueue.h"
#include "QueueAnalyzer.h"

int main()
{
	MessageQueue<std::string> Test(3);
	QueueAnalyzer<std::string> t(Test);
	auto start = std::chrono::system_clock::now();

	// 1 = highest priority
	Test.push({ 4, start + std::chrono::seconds(2), "World" });
	Test.push({ 1, start + std::chrono::seconds(3), "Hello" });
	Test.push({ 5, start + std::chrono::seconds(7), "Goodbye" });

	t.Analyzer("output.txt");

	// sorted by priority => return Hello
	std::cout << Test.getMessage() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));

	// World is expired => return Goodbye
	std::cout << Test.getMessage() << std::endl;

	// empty queue => return nothing
	std::cout << Test.getMessage() << std::endl;

	Test.push({ 5, std::chrono::system_clock::now() + std::chrono::seconds(3), "World2" });
	t.Analyzer("output.txt");

	// already expired => won't be added 
	Test.push({ 1, start + std::chrono::seconds(1), "Hello2" });

	Test.push({ 2, std::chrono::system_clock::now() + std::chrono::seconds(2), "Goodbye2" });
	std::cout << Test.getMessage() << std::endl;

	/*MessageQueue<int> Test2(3);
	auto start2 = std::chrono::system_clock::now();
	Test2.push({ 4, start2 + std::chrono::seconds(3), 4 });
	Test2.push({ 1, start2 + std::chrono::seconds(3), 1 });
	Test2.push({ 5, start2 + std::chrono::seconds(6), 6 });
	std::cout << Test2.getMessage() << std::endl;*/
}