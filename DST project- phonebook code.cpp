#include <iostream>
#include <string>
#include <fstream>

using namespace std;


const char* phonebook_filename = "phone_book.txt";


enum groups {Family, Friends, Colleagues, VIP, Others};


struct contact_info
{
	string phone_number;
	string contact_name;
	groups contact_group;

	contact_info* next_element=NULL;
};

contact_info* phonebook=NULL;


unsigned int N_entries = 0;


contact_info* look_up[26] = {NULL};

void upcase (string&);		

contact_info* create_entry();	

void contact_insert(contact_info*);

contact_info* contact_find(string);

void contact_show(contact_info);	

void contact_find_show_delete();	

void contact_find_show();	

void phonebook_save();			

void phonebook_load();			

int main()
{
	// Reading phonebook 
	phonebook_load();
	
	
	char userchoice;
	do
	{
		cout << endl;
		cout << "a. Create a new entry." << endl;
		cout << "b. Delete an entry." << endl;
		cout << "c. Find an entry and show its information. " << endl;
		cout << "d. Save the whole phonebook in a file. " << endl;
		cout << "e. Exit. " << endl;
		cout << endl << "Please enter your choice: ";
		cin >> userchoice;
		switch (userchoice)
		{
		
		
		case 'a': case 'A':
		
			contact_insert(create_entry());
			break;
		case 'b': case 'B':
		
			contact_find_show_delete();
			break;
		
		
		case 'c': case 'C':
			contact_find_show();
			break;
		
		
		case 'd': case 'D': case 'e': case 'E':
			phonebook_save();
			break;
		
		default:
			cout << endl << "!!! Invalid choice !!!" << endl;
		}

	
	} while ((userchoice != 'e') && (userchoice != 'E'));
	cout << "Exiting program..." << endl;
	return 0;
}

void upcase(string &str)
{
	for (int i=0; i<str.length(); i++) str[i]=toupper(str[i]);
}


contact_info* create_entry()
{
	// Creating a new blank contact
	contact_info* new_entry = new contact_info;
	cout << "Please enter name: ";
	cin.ignore();	
	getline (cin,new_entry->contact_name);
	upcase(new_entry->contact_name);
	cout << "Please enter phone number: ";
	getline (cin,new_entry->phone_number);
	upcase(new_entry->phone_number);
	cout << "Please enter group" << endl;
	cout << "1, f or F = Family" << endl;
	cout << "2, r or R = Friends" << endl;
	cout << "3, c or C = Colleagues" << endl;
	cout << "4, c or c = VIP" << endl;
	cout << "or enter any other character for (Others) group" << endl;
	cout << "Your choice: ";
	char groupchoice;
	cin >> groupchoice;
	switch (groupchoice)
	{
	case '1': case 'f': case 'F':
		new_entry->contact_group=Family;
		break;
	case '2': case 'r': case 'R':
		new_entry->contact_group=Friends;
		break;
	case '3': case 'c': case 'C':
		new_entry->contact_group=Colleagues;
		break;
	case '4': case 'v': case 'V':
		new_entry->contact_group=VIP;
		break;
	default:
		new_entry->contact_group=Others;
	}
	new_entry->next_element=NULL;
	cout << "New contact successfully created." << endl;
	return new_entry;
}


void contact_insert(contact_info* newcontact)
{
	
	if (N_entries==0 || phonebook->contact_name.compare(newcontact->contact_name)>0)
	{
		newcontact->next_element=phonebook;
		phonebook=newcontact;
	}
	else
	{
		contact_info* previous=phonebook;
		contact_info* next=phonebook->next_element;
		while(next!=NULL)
		{
			if (newcontact->contact_name.compare(next->contact_name)<0) break;
			previous=next;
			next=next->next_element;
		}
		previous->next_element=newcontact;
		newcontact->next_element=next;		
	}	
	N_entries++;
}


contact_info* contact_find(string Name)
{
	contact_info* contact = phonebook;
	while (contact != NULL)
	{
		if (Name.compare(contact->contact_name)==0)
			return contact;
		else
			contact=contact->next_element;
	}
	return NULL;
}


void contact_show(contact_info* contact)
{
	cout << "Contact Name: " << contact->contact_name << endl;
	cout << "Contact Number: " << contact->phone_number << endl;
	cout << "Contact Group: ";
	switch (contact->contact_group)
	{
		case Family: cout << "FAMILY" << endl; break;
		case Friends: cout << "FRIENDS" << endl; break;
		case Colleagues: cout << "COLLEAGUES" << endl; break;
		case VIP: cout << "VIP" << endl; break;
		default: cout << "OTHERS" << endl;
	}
}


void contact_find_show_delete()
{
	cout << "Please enter contact name to be deleted: ";
	string Name;
	cin.ignore();
	getline (cin,Name);
	upcase(Name);
	cout << endl;
	contact_info* contact = contact_find(Name);
	if(contact != NULL)
	{
		cout << "We are going to delete following contact:" << endl;
		contact_show(contact);
		cout << "Enter Y/y to confirm delete:";
		char confirm;
		cin >> confirm;
		if (confirm == 'Y' || confirm == 'y')
		{
			//for best case or we can say deleting contact is first 
			if (contact==phonebook)
			{
				phonebook=phonebook->next_element;
			}
			// Else, we should find previous contact for linking
			else
			{
				contact_info* previous=phonebook;
				while(previous->next_element != contact) previous=previous->next_element;
				previous->next_element=contact->next_element;				
			}
			// Free memory for deleted contact
			delete contact;
			N_entries--;
			cout << "Contact successfully deleted" << endl;
		}
		else cout << "Did not delete contact." << endl;
	}
	else cout << "Did not found " << Name << " in phonebook!!!" << endl;
}


void contact_find_show()
{
	cout << "Please enter contact name to be searched for: ";
	string Name;
	cin.ignore();
	getline (cin,Name);
	cout << endl;
	upcase(Name);
	contact_info* contact = contact_find(Name);
	if(contact != NULL)
		contact_show(contact);
	else
		cout << "Did not found " << Name << " in phonebook!!!" << endl;
}


void phonebook_save()
{
	ofstream phonebook_file;
	// --- called "phone_book.txt".
	phonebook_file.open(phonebook_filename);
	cout << "Writing " << N_entries << " records to " << phonebook_filename << " ..." << endl;
	// --- On the first line the number of entries must be saved
	phonebook_file << N_entries << endl;
	contact_info* current_item=phonebook;
	while (current_item != NULL)
	{
		// --- Entries must be separated from each other and from the first line by a line of twenty *s (********************).
		phonebook_file << "********************" << endl;
		// ---  For each entry, the name must be stored on the first line
		phonebook_file << current_item->contact_name << endl;
		phonebook_file << current_item->phone_number << endl;
		switch (current_item->contact_group)
		{
			case Family: phonebook_file << "FAMILY" << endl; break;
			case Friends: phonebook_file << "FRIENDS" << endl; break;
			case Colleagues: phonebook_file << "COLLEAGUES" << endl; break;
			case VIP: phonebook_file << "VIP" << endl; break;
			default: phonebook_file << "OTHERS" << endl;
		}
		current_item = current_item->next_element;
	}
	phonebook_file.close();
}


void phonebook_load()
{
	ifstream phonebook_file;
	phonebook_file.open(phonebook_filename);
	if(!phonebook_file.is_open())
	{
		cout << "Phonebook file could not be openned !!!" << endl;
	} else
	{
		phonebook_file >> N_entries;
		contact_info** previous=&phonebook;
		string text;
		getline (phonebook_file,text);
		cout << "Reading " << N_entries << " of contact records..." << endl;
		for (int i=0;i<N_entries;i++)
		{
			contact_info* new_entry = new contact_info;
		
			getline (phonebook_file,text);
			getline (phonebook_file,new_entry->contact_name);
			getline (phonebook_file,new_entry->phone_number);
			getline (phonebook_file,text);
			if      (text.compare("FAMILY") == 0)  	new_entry->contact_group=Family;
			else if (text.compare("FRIENDS") == 0)	new_entry->contact_group=Friends;
			else if (text.compare("COLLEAGUES")==0)	new_entry->contact_group=Colleagues;
			else if (text.compare("VIP") == 0)	new_entry->contact_group=VIP;
			else new_entry->contact_group=Others;
			new_entry->next_element=NULL;
			*previous=new_entry;
			previous=&new_entry->next_element;
		}
		phonebook_file.close();
	}
}


