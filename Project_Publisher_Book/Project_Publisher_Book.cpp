#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>

using namespace std;

int main()
{
	struct PublisherType
	{
		int p_id;
		string p_name;
		string p_address;
	};
	PublisherType Publisher_record;
	vector<PublisherType> Publisher;

	struct bookType
	{
		int b_id;
		string b_author;
		string b_title;
		string b_isbn;
		string b_type;
		double b_price;
		int p_id;
	};
	bookType Book_record;
	vector<bookType> Book;

	char discard;

	ifstream inDataB, inDataP;
	ofstream outData;

	inDataB.open("Book.in");
	inDataP.open("Publisher.in");
	outData.open("Pub_Book.out");

	cout << "\nReading Values From Publisher.in File\n\n";

	inDataP >> Publisher_record.p_id;
	inDataP.get(discard);
	getline(inDataP, Publisher_record.p_name, '\t');
	getline(inDataP, Publisher_record.p_address, '\t');
	inDataP.get(discard);

	while (inDataP)
	{
		Publisher.push_back( Publisher_record);

		inDataP >> Publisher_record.p_id;
		inDataP.get(discard);
		getline(inDataP, Publisher_record.p_name, '\t');
		getline(inDataP, Publisher_record.p_address, '\t');
		inDataP.get(discard);
	}

	inDataP.close();

	cout << "\nReading Values From Book.in File\n\n";
	
	inDataB >> Book_record.b_id;
	inDataB.get(discard);
	getline(inDataB, Book_record.b_author, '\t');
	getline(inDataB, Book_record.b_title, '\t');
	getline(inDataB, Book_record.b_isbn, '\t');
	getline(inDataB, Book_record.b_type, '\t');
	inDataB >> Book_record.b_price;
	inDataB.get(discard);
	inDataB >> Book_record.p_id;
	inDataB.get(discard);

	while (inDataB)
	{
		Book.push_back(Book_record);

		inDataB >> Book_record.b_id;
		inDataB.get(discard);
		getline(inDataB, Book_record.b_author, '\t');
		getline(inDataB, Book_record.b_title, '\t');
		getline(inDataB, Book_record.b_isbn, '\t');
		getline(inDataB, Book_record.b_type, '\t');
		inDataB >> Book_record.b_price;
		inDataB.get(discard);
		inDataB >> Book_record.p_id;
		inDataB.get(discard);
	}

	inDataB.close();	

	cout << "\n\nDisplay the components of Book and Publisher variables of Struct Array type:\n\n";

	for (int i = 0; i < Publisher.size(); i++)
	{
			cout << i + 1 << ")Publisher\'s ID: " << Publisher[i].p_id
			     << ", Publisher\'s Name: " << Publisher[i].p_name
			     << ",\n  Publisher\'s Address: " << Publisher[i].p_address 
				 << "\n\t\thas published the following Books:\n\n";
			outData << i + 1 << ")Publisher\'s ID: " << Publisher[i].p_id
			    	<< ", Publisher\'s Name: " << Publisher[i].p_name
					<< ",\n  Publisher\'s Address: " << Publisher[i].p_address
					<< "\n\t\thas published the following Books:\n\n";
			for (int j = 0; j < Book.size(); j++)
			{
				if (Book[j].p_id == Publisher[i].p_id)
				{
					cout << "\tBook\'s ID: " << Book[j].b_id
					  	 << ", Author: " << Book[j].b_author
						 << ", Book\'s Title: " << Book[j].b_title << "\n\n";
					outData << "\tBook\'s ID: " << Book[j].b_id
					  	    << ", Author: " << Book[j].b_author
						    << ", Book\'s Title: " << Book[j].b_title << "\n\n";
				}
			}
	}

	outData.close();

	cout << "\n\nPress any key to countinue . . . ";
	cin >> discard;
	return 0;
}