#include "pch.h"
#include "BackBeat/Audio/Helpers/Snippets.h"

namespace BackBeat {

	namespace SnippetTests
	{
		TEST(TestSnippet, DefaultConstructorSize)
		{
			Snippets snippets;
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, DefaultConstructorMin)
		{
			Snippets snippets;
			EXPECT_EQ(snippets.GetMin(), 0);
		}

		TEST(TestSnippet, DefaultConstructorMax)
		{
			Snippets snippets;
			EXPECT_EQ(snippets.GetMax(), 1);
		}

		TEST(TestSnippet, SetMax1)
		{
			Snippets snippets;

			EXPECT_FALSE(snippets.SetMax(0));
			EXPECT_EQ(snippets.GetMax(), 1);
		}

		TEST(TestSnippet, SetMax2)
		{
			Snippets snippets;

			EXPECT_TRUE(snippets.SetMax(1));
			EXPECT_EQ(snippets.GetMax(), 1);
		}

		TEST(TestSnippet, SetMax3)
		{
			Snippets snippets;

			EXPECT_TRUE(snippets.SetMax(10));
			EXPECT_EQ(snippets.GetMax(), 10);
		}

		TEST(TestSnippet, SetMax4)
		{
			Snippets snippets;

			snippets.SetMax(10);

			EXPECT_TRUE(snippets.SetMax(1));
			EXPECT_EQ(snippets.GetMax(), 1);
		}

		TEST(TestSnippet, SetMin1)
		{
			Snippets snippets;

			EXPECT_FALSE(snippets.SetMin(1));
			EXPECT_EQ(snippets.GetMin(), 0);
		}

		TEST(TestSnippet, SetMin2)
		{
			Snippets snippets;

			snippets.SetMax(100);

			EXPECT_TRUE(snippets.SetMin(1));
			EXPECT_EQ(snippets.GetMin(), 1);
		}

		TEST(TestSnippet, SetMin3)
		{
			Snippets snippets;

			snippets.SetMax(100);

			EXPECT_TRUE(snippets.SetMin(99));
			EXPECT_EQ(snippets.GetMin(), 99);
		}

		TEST(TestSnippet, SetMin4)
		{
			Snippets snippets;

			snippets.SetMax(100);

			EXPECT_FALSE(snippets.SetMin(100));
			EXPECT_EQ(snippets.GetMin(), 0);
		}

		TEST(TestSnippet, AddFailure1)
		{
			Snippets snippets;

			EXPECT_EQ(snippets.Add(nullptr), -1);
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, AddFailure2)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 0;
			snippet->end = 2;

			EXPECT_EQ(snippets.Add(snippet), -1);
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, AddFailure3)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 3;
			snippet->end = 2;

			EXPECT_EQ(snippets.Add(snippet), -1);
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, AddFailure4)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 1;
			snippet->end = 1;

			EXPECT_EQ(snippets.Add(snippet), -1);
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, AddFailure5)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 3;
			snippet->end = 10;

			EXPECT_EQ(snippets.Add(snippet), -1);
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, AddFailure6)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 0;
			snippet->end = 1;

			snippets.Add(snippet);

			EXPECT_EQ(snippets.Add(snippet), -1);
			EXPECT_EQ(snippets.GetSize(), 1);
		}

		TEST(TestSnippet, AddFailure7)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 0;
			snippet1->end = 50;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet1->start = 49;
			snippet1->end = 51;

			EXPECT_EQ(snippets.Add(snippet2), -1);
			EXPECT_EQ(snippets.GetSize(), 1);
		}

		TEST(TestSnippet, AddSuccess1)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 0;
			snippet->end = 1;

			EXPECT_EQ(snippets.Add(snippet), 0);
			EXPECT_EQ(snippets.GetSize(), 1);
		}

		TEST(TestSnippet, AddSuccess2)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 0;
			snippet1->end = 50;

			EXPECT_EQ(snippets.Add(snippet1), 0);
			EXPECT_EQ(snippets.GetSize(), 1);
		}

		TEST(TestSnippet, AddSuccess3)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 0;
			snippet1->end = 50;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 51;
			snippet2->end = 100;

			EXPECT_EQ(snippets.Add(snippet2), 1);
			EXPECT_EQ(snippets.GetSize(), 2);
		}

		TEST(TestSnippet, AddSuccess4)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 48;

			EXPECT_EQ(snippets.Add(snippet2), 0);
			EXPECT_EQ(snippets.GetSize(), 2);
		}

		TEST(TestSnippet, AddSuccess5)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet = nullptr;
			int start = 0;
			int end = 1;
			for (int i = 0; i < 49; i++)
			{
				snippet = std::make_shared<Snippet>();
				snippet->start = start;
				snippet->end = end;
				snippets.Add(snippet);
				snippet = nullptr;
				start += 2;
				end += 2;
			}

			EXPECT_EQ(snippets.GetSize(), 49);
		}

		TEST(TestSnippet, AddSuccess6)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;

			EXPECT_EQ(snippets.Add(snippet3), 1);
			EXPECT_EQ(snippets.GetSize(), 3);
		}

		TEST(TestSnippet, DeleteFailure1)
		{
			Snippets snippets;

			EXPECT_FALSE(snippets.Delete(0));
		}

		TEST(TestSnippet, DeleteFailure2)
		{
			Snippets snippets;

			EXPECT_FALSE(snippets.Delete(10));
		}

		TEST(TestSnippet, DeleteFailure3)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 0;
			snippet->end = 1;

			snippets.Add(snippet);
			EXPECT_FALSE(snippets.Delete(1));
			EXPECT_EQ(snippets.GetSize(), 1);
		}

		TEST(TestSnippet, DeleteSuccess1)
		{
			Snippets snippets;

			std::shared_ptr<Snippet> snippet = std::make_shared<Snippet>();
			snippet->start = 0;
			snippet->end = 1;

			snippets.Add(snippet);
			EXPECT_TRUE(snippets.Delete(0));
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, DeleteSuccess2)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			EXPECT_TRUE(snippets.Delete(0));
			EXPECT_EQ(snippets.GetSize(), 2);
			EXPECT_TRUE(snippets.Delete(0));
			EXPECT_EQ(snippets.GetSize(), 1);
			EXPECT_TRUE(snippets.Delete(0));
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, DeleteSuccess3)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			EXPECT_TRUE(snippets.Delete(2));
			EXPECT_EQ(snippets.GetSize(), 2);
			EXPECT_TRUE(snippets.Delete(1));
			EXPECT_EQ(snippets.GetSize(), 1);
			EXPECT_TRUE(snippets.Delete(0));
			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, DeleteSuccess4)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			EXPECT_TRUE(snippets.Delete(1));
			EXPECT_EQ(snippets.GetSize(), 2);
		}

		TEST(TestSnippet, DeleteSuccess5)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet = nullptr;
			int start = 0;
			int end = 1;
			for (int i = 0; i < 49; i++)
			{
				snippet = std::make_shared<Snippet>();
				snippet->start = start;
				snippet->end = end;
				snippets.Add(snippet);
				snippet = nullptr;
				start += 2;
				end += 2;
			}

			for (int i = 0; i < 49; i++)
			{
				snippets.Delete(0);
			}

			EXPECT_EQ(snippets.GetSize(), 0);
		}

		TEST(TestSnippet, GetFailure1)
		{
			Snippets snippets;

			EXPECT_EQ(snippets.Get(0), nullptr);
		}

		TEST(TestSnippet, GetFailure2)
		{
			Snippets snippets;

			EXPECT_EQ(snippets.Get(100), nullptr);
		}

		TEST(TestSnippet, GetFailure3)
		{
			Snippets snippets;

			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			EXPECT_EQ(snippets.Get(1), nullptr);
		}

		TEST(TestSnippet, GetFailure4)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			EXPECT_EQ(snippets.Get(3), nullptr);
		}

		TEST(TestSnippet, GetSuccess1)
		{
			Snippets snippets;

			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippet1->on = false;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = snippets.Get(0);
			EXPECT_EQ(snippet1->start, snippet2->start);
			EXPECT_EQ(snippet1->end, snippet2->end);
			EXPECT_EQ(snippet1->on, snippet2->on);
		}

		TEST(TestSnippet, GetSuccess2)
		{
			Snippets snippets;

			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet0 = std::make_shared<Snippet>();
			snippet0->start = 51;
			snippet0->end = 101;
			snippet0->on = false;
			snippets.Add(snippet0);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippet1->on = false;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = snippets.Get(0);
			EXPECT_EQ(snippet1->start, snippet2->start);
			EXPECT_EQ(snippet1->end, snippet2->end);
			EXPECT_EQ(snippet1->on, snippet2->on);
		}

		TEST(TestSnippet, GetSuccess3)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippetToCheck->start, snippet2->start);
			EXPECT_EQ(snippetToCheck->end, snippet2->end);
			EXPECT_EQ(snippetToCheck->on, snippet2->on);

			snippetToCheck = snippets.Get(1);
			EXPECT_EQ(snippetToCheck->start, snippet3->start);
			EXPECT_EQ(snippetToCheck->end, snippet3->end);
			EXPECT_EQ(snippetToCheck->on, snippet3->on);

			snippetToCheck = snippets.Get(2);
			EXPECT_EQ(snippetToCheck->start, snippet1->start);
			EXPECT_EQ(snippetToCheck->end, snippet1->end);
			EXPECT_EQ(snippetToCheck->on, snippet1->on);
		}

		TEST(TestSnippet, GetSuccess4)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet = nullptr;
			int start = 0;
			int end = 1;
			for (int i = 0; i < 49; i++)
			{
				snippet = std::make_shared<Snippet>();
				snippet->start = start;
				snippet->end = end;
				snippets.Add(snippet);
				snippet = nullptr;
				start += 2;
				end += 2;
			}

			for (int i = 0; i < 49; i++)
			{
				EXPECT_NE(nullptr, snippets.Get(i));
			}

		}

		TEST(TestSnippet, SetMinUpdate1)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMin(99);
			EXPECT_EQ(99, snippets.GetMin());
			EXPECT_EQ(0, snippets.GetSize());
		}

		TEST(TestSnippet, SetMinUpdate2)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippet1->on = true;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippet2->on = true;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippet3->on = true;
			snippets.Add(snippet3);

			snippets.SetMin(50);
			std::shared_ptr<Snippet> snippetToCheck = snippets.Get(0);

			EXPECT_EQ(50, snippets.GetMin());
			EXPECT_EQ(1, snippets.GetSize());
			EXPECT_EQ(snippet1->start, snippetToCheck->start);
			EXPECT_EQ(snippet1->end, snippetToCheck->end);
			EXPECT_EQ(snippet1->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinUpdate3)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippet1->on = true;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippet2->on = true;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippet3->on = true;
			snippets.Add(snippet3);

			snippets.SetMin(40);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(40, snippets.GetMin());
			EXPECT_EQ(2, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet3->start, snippetToCheck->start);
			EXPECT_EQ(snippet3->end, snippetToCheck->end);
			EXPECT_EQ(snippet3->on, snippetToCheck->on);

			snippetToCheck = snippets.Get(1);
			EXPECT_EQ(snippet1->start, snippetToCheck->start);
			EXPECT_EQ(snippet1->end, snippetToCheck->end);
			EXPECT_EQ(snippet1->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMaxUpdate1)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMax(1);
			EXPECT_EQ(1, snippets.GetMax());
			EXPECT_EQ(0, snippets.GetSize());
		}

		TEST(TestSnippet, SetMaxUpdate2)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippet1->on = true;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippet2->on = true;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippet3->on = true;
			snippets.Add(snippet3);

			snippets.SetMax(40);
			std::shared_ptr<Snippet> snippetToCheck = snippets.Get(0);

			EXPECT_EQ(40, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());
			EXPECT_EQ(snippet2->start, snippetToCheck->start);
			EXPECT_EQ(snippet2->end, snippetToCheck->end);
			EXPECT_EQ(snippet2->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMaxUpdate3)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippet1->on = true;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippet2->on = true;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippet3->on = true;
			snippets.Add(snippet3);

			snippets.SetMax(50);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(50, snippets.GetMax());
			EXPECT_EQ(2, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet2->start, snippetToCheck->start);
			EXPECT_EQ(snippet2->end, snippetToCheck->end);
			EXPECT_EQ(snippet2->on, snippetToCheck->on);

			snippetToCheck = snippets.Get(1);
			EXPECT_EQ(snippet3->start, snippetToCheck->start);
			EXPECT_EQ(snippet3->end, snippetToCheck->end);
			EXPECT_EQ(snippet3->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinMaxUpdate1)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMin(41);
			snippets.SetMax(50);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(41, snippets.GetMin());
			EXPECT_EQ(50, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet3->start, snippetToCheck->start);
			EXPECT_EQ(snippet3->end, snippetToCheck->end);
			EXPECT_EQ(snippet3->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinMaxUpdate2)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 0;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMax(50);
			snippets.SetMin(41);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(41, snippets.GetMin());
			EXPECT_EQ(50, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet3->start, snippetToCheck->start);
			EXPECT_EQ(snippet3->end, snippetToCheck->end);
			EXPECT_EQ(snippet3->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinMaxUpdate3)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 20;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMin(20);
			snippets.SetMax(40);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(20, snippets.GetMin());
			EXPECT_EQ(40, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet2->start, snippetToCheck->start);
			EXPECT_EQ(snippet2->end, snippetToCheck->end);
			EXPECT_EQ(snippet2->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinMaxUpdate4)
		{
			Snippets snippets;
			snippets.SetMax(100);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 20;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMax(40);
			snippets.SetMin(20);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(20, snippets.GetMin());
			EXPECT_EQ(40, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet2->start, snippetToCheck->start);
			EXPECT_EQ(snippet2->end, snippetToCheck->end);
			EXPECT_EQ(snippet2->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinMaxUpdate5)
		{
			Snippets snippets;
			snippets.SetMax(150);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 20;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMin(50);
			snippets.SetMax(100);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(50, snippets.GetMin());
			EXPECT_EQ(100, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet1->start, snippetToCheck->start);
			EXPECT_EQ(snippet1->end, snippetToCheck->end);
			EXPECT_EQ(snippet1->on, snippetToCheck->on);
		}

		TEST(TestSnippet, SetMinMaxUpdate6)
		{
			Snippets snippets;
			snippets.SetMax(150);

			std::shared_ptr<Snippet> snippet1 = std::make_shared<Snippet>();
			snippet1->start = 51;
			snippet1->end = 100;
			snippets.Add(snippet1);

			std::shared_ptr<Snippet> snippet2 = std::make_shared<Snippet>();
			snippet2->start = 20;
			snippet2->end = 40;
			snippets.Add(snippet2);

			std::shared_ptr<Snippet> snippet3 = std::make_shared<Snippet>();
			snippet3->start = 41;
			snippet3->end = 50;
			snippets.Add(snippet3);

			snippets.SetMax(100);
			snippets.SetMin(50);
			std::shared_ptr<Snippet> snippetToCheck = nullptr;

			EXPECT_EQ(50, snippets.GetMin());
			EXPECT_EQ(100, snippets.GetMax());
			EXPECT_EQ(1, snippets.GetSize());

			snippetToCheck = snippets.Get(0);
			EXPECT_EQ(snippet1->start, snippetToCheck->start);
			EXPECT_EQ(snippet1->end, snippetToCheck->end);
			EXPECT_EQ(snippet1->on, snippetToCheck->on);
		}
	}
}