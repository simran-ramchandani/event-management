#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class event
{
    int event_id;
    string event_name;
    int event_date[3];
    float budget;
    string venue;
    static string status_opt[4];
    string status;

public:
    event();
    event(int id);
    void update_event();
    void cancel_event();
    static void print_event();
    float get_budget(){return budget;}
    friend class BudgetManager;
};

string event::status_opt[4] = {"Started", "Discarded", "Postponed", "Completed"};
event::event()
{
    event_id = 0;
    event_name = "Unnamed Event";
    budget = 0.0;
    venue = "TBD";
    status = status_opt[0];
    
    event_date[0] = 1;
    event_date[1] = 1;
    event_date[2] = 2025;
}
event::event(int id)
{
    bool run=true;
    event_id=id;
    string check;
    string v[] = {"Grand Hall", "Riverside Pavilion", "Skyline Terrace", "Crystal Ballroom", "Garden Courtyard"};
    int choice;
    cout<<"Welcome to new EVENT:"<<endl;
    cout<<"Enter event name: "<<endl;
    cin.ignore();
    getline(cin, event_name);
    while(run)
    {
        cout<<"\nEnter Date of event(dd mm yyyy): "<<endl;
        cin>>event_date[0]>>event_date[1]>>event_date[2];
        if(event_date[0]<0&&event_date[0]>31)
            cout<<"Wrong date entered. Try again."<<endl;
        else if(event_date[1]<0&&event_date[1]>12)
            cout<<"Wrong date entered. Try again."<<endl;
        else if(event_date[2]<2025)
            cout<<"Wrong date entered. Try again."<<endl;
        else
        {
            run=false;
        }
    }
    
    
    cout<<"What budget do you have in mind? "<<endl;
    cin>>budget;
    cout<<"Do you have a venue in mind? (yes/no)"<<endl;
    cin>>check;
    if (check == "yes") 
    {
        cout << "Enter the venue: ";
        cin.ignore();
        getline(cin, venue);
    } 
    else if (check == "no") 
    {
        cout << "Available venues:\n";
        for (int i = 0; i < 5; i++) 
        {
            cout << i + 1 << ". " << v[i] << endl;
        }
        
        cout << "Choose one of the above (enter the number): ";
        cin >> choice;
        
        if (choice >= 1 && choice <= 5) 
        {
            venue = v[choice - 1];
            cout << "You have selected: " << venue << endl;
        } 
        else 
        {
            cout << "Invalid input DIE" << endl;
        }
    } 
    else 
    {
        cout << "Invalid input DIE" << endl;
    }
    status=status_opt[0];

    ofstream file("events.csv",ios::app);
    if (!file.is_open())
    {
        cout << "Error opening events.csv!" << endl;
        return;
    }

    file << event_name << "," << event_date[0] << "/" << event_date[1] << "/" << event_date[2]
         << "," << budget << "," << venue << "," << status << "\n";

    file.close();
}
void event::update_event()
{
    int flag;
    cout<<"What parameter has been changed?"<<endl;
    cout<<"1.Name \n2.Budget\n3.Venue"<<endl;
    cin>>flag;
    if(flag==1)
    {
        cout<<"Enter New Name for the event"<<endl;
        cin.ignore();
        getline(cin, event_name);
        cout<<"Event name updated!"<<endl;
    }
    else if(flag==2)
    {
        cout<<"Enter updated Budget: "<<endl;
        cin>>budget;
        cout<<"Budget updated!"<<endl;
    }
    else if(flag==3)
    {
        cout<<"Enter change of venue: "<<endl;
        cin.ignore();
        getline(cin, venue);
        cout<<"Venue updated!"<<endl;
    }
    else
    {
        cout<<"Invalid Choice"<<endl;
    }
}
void event::cancel_event()
{
    int check;
    cout<<"Are you sure you want to cancel event?(1/0)"<<endl;
    cin>>check;
    if(check==1)
    {
        status = status_opt[1];
        cout<<"Event has been canceled:("<<endl;
    }
    else if(check==0)
    {
        cout<<"Event hasn't been canceled"<<endl;
    }
}
void event::print_event()
{
    int i=0;
    ifstream file("events.csv"); 
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    string line;
    cout << "\n--- List of Events ---\n";
    while (getline(file, line))
    {
        cout << i+1 << ". " << line << endl;
        i++;
    }

    file.close();
}

class Schedule:public event
{
    int schedule_id, activity_count;
    int event_id;
    string activity_name[20];
    string start_time[20];
    string end_time[20];
    string status[20];
    
    bool validate_schedule(string start_time, string end_time);

    public:
    Schedule() : event(), activity_count(0) {} 
    void create_schedule(int event_id);
    void update_schedule(int schedule_id);
    void print_schedule();
};
void Schedule::create_schedule(int event_id)
{
    cout<<"What is the name of this Activity?"<<endl;
    cin.ignore();
    getline(cin, activity_name[activity_count]);
    cout<<"What time will the activity Start?(HH:MM)"<<endl;
    cin>>start_time[activity_count];
    cout<<"what time will the activity end?(HH:MM)"<<endl;
    cin>>end_time[activity_count];
    if(!validate_schedule(start_time[activity_count], end_time[activity_count]))
    {
        return;
    }
    status[activity_count] = "Scheduled";
    activity_count++;
}
void Schedule::update_schedule(int event_id)
{
    print_schedule();
    int check, choice;
    cout<<"Enter which number you want to update"<<endl;
    cin>>check;
    if (check < 1 || check > activity_count)
    {
        cout << "Invalid index!\n";
        return;
    }
    check--;
    cout << "What do you want to update?"<<endl;
    cout<<"\n1. Name\n2. Start Time\n3. End Time\n4. Status"<<endl;
    cin >> choice;
    switch (choice)
    {
    case 1:
        cout<<"Enter new name: "<<endl;
        cin.ignore();
        getline(cin, activity_name[check]);
        break;
    case 2:
        cout<<"Enter new Start Time: "<<endl;
        cin>>start_time[check];
        break;
    case 3:
        cout<<"Enter new End Time: "<<endl;
        cin>>end_time[check];
        break;
    case 4:
        cout<<"Update Status: "<<endl;
        cin>>status[check];
        break;
    default:
        break;
    }
}
bool Schedule::validate_schedule(string start_time, string end_time)
{
    int s_hour, s_minute, e_hour, e_minute;
    char colon;
    stringstream ss_start(start_time);
    stringstream ss_end(end_time);
    
    if (!(ss_start >> s_hour >> colon >> s_minute) || colon != ':' || s_hour < 0 || s_hour > 23 || s_minute< 0 || s_minute > 59)
    {
        cout << "Wrong Format for start time (00-23 for hours, 00-59 for minutes)." << endl;
        return false;
    }
    if (!(ss_end >> e_hour >> colon >> e_minute) || colon != ':' || e_hour < 0 || e_hour > 23 || e_minute < 0 || e_minute > 59)
    {
        cout << "Wrong Format for end time (00-23 for hours 00-59 for minutes)" << endl;
        return false;
    }
    if (s_hour > e_hour || (s_hour == e_hour && s_minute >= e_minute))
    {
        cout << "End time must be after start time!" << endl;
        return false;
    }
    return true;
}
void Schedule::print_schedule()
{
    if (activity_count == 0) {
        cout << "No activities scheduled yet." << endl;
        return;
    }
    
    for(int i = 0; i < activity_count; i++)
    {
        cout << i+1 << ". " << start_time[i] << " - " << end_time[i] << ": " << activity_name[i] << " (" << status[i] << ")" << endl;
    }
}

class Task : public Schedule
{
    int task_id, pri; 
    string task_name;
    string assigned_to;
    string due_date[3];
    string stat;
    static string priority[3];
    static string status[4];
    
    public:
    Task() : task_id(0), pri(0), stat("") {} // Initialize with default values
    void assign_task();
    void track_progress();
    void mark_complete();
};
string Task::priority[3] = {"High Priority", "Medium Priority", "Low Priority"};
string Task::status[4] = {"Started", "Discarded", "Postponed", "Completed"};
void Task::assign_task()
{
    cout << "Enter Details of task: " << endl;
    cin.ignore();
    getline(cin, task_name);
    
    cout << "Who do you want to assign task to: " << endl;
    getline(cin, assigned_to);
    
    cout << "What is the due date of the task(dd mm yyyy): " << endl;
    cin >> due_date[0] >> due_date[1] >> due_date[2];
    
    cout << "Assign Priority to the task: " << endl;
    for(int i = 0; i < 3; i++)
    {
        cout << i+1 << "." << priority[i] << endl;
    }
    cin >> pri;
    
    if (pri >= 1 && pri <= 3) {
        stat = priority[pri-1];
        cout << "Task assigned with " << stat << endl;
    } else {
        cout << "Invalid priority. Setting to Medium Priority by default." << endl;
        stat = priority[1];
    }
}
void Task::track_progress()
{
    if (task_name.empty()) {
        cout << "No task assigned yet!" << endl;
        return;
    }
    
    cout << "Task: " << task_name << endl;
    cout << "Assigned To: " << assigned_to << endl;
    cout << "Due date: " << due_date[0] << "/" << due_date[1] << "/" << due_date[2] << endl;
    cout << "Priority: " << stat << endl;
}
void Task::mark_complete()
{
    if (task_name.empty()) {
        cout << "No task assigned yet!" << endl;
        return;
    }
    
    int check;
    cout << "Do you wanna change status of task?" << endl;
    track_progress();
    cout << "Enter 1 for Started:" << endl;
    cout << "Enter 2 for Discarded:" << endl;
    cout << "Enter 3 for Postponed:" << endl;
    cout << "Enter 4 for Completed:" << endl;
    cin >> check;
    
    if(check >= 1 && check <= 4)
    {
        stat = status[check-1];
        cout << "Task has been marked as " << stat << endl;
    }
    else
    {
        cout << "Invalid choice!" << endl;
    }
}

class vendor : public Task
{
private:
    string name, service, contact, status;

public:
    vendor()
    {
        name = "No data yet";
        service = "No data yet";
        contact = "No data yet";
        status = "No data yet";
    }
    
    void getdata()
    {
        cout << "Enter details of vendors:" << endl;
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Service: ";
        getline(cin, service);
        cout << "Enter Contact detail: ";
        getline(cin, contact);
        cout << "Enter Status: ";
        getline(cin, status);
    }
    
    void confirm_service()
    {
        if (name == "No data yet") {
            cout << "No vendor data available. Please add vendor details first." << endl;
            return;
        }
        
        status = "Confirmed";
        cout << name << " Confirmed service for " << service << endl;
    }
    
    void display_info()
    {
        cout << "Vendor Name: " << name << endl;
        cout << "Service: " << service << endl;
        cout << "Contact: " << contact << endl;
        cout << "Status: " << status << endl;
        cout << endl;
    }
};

class Client:public event 
{
    string clientName, contactInfo;

    public:
    Client()
    {
        clientName = "NA";
        contactInfo = "NA";
    }
    
    void addClient() 
    {
        cout << "Enter Client Name: ";
        getline(cin, clientName);
        cout << "Enter Contact Info: ";
        getline(cin, contactInfo);
        cout << "Client Added Successfully!" << endl;
    }

    void displayClient() 
    {
        cout << "\nCLIENT DETAILS \n";
        cout << "Name: " << clientName << "\nContact Info: " << contactInfo << "\n";
    }
};

class BudgetManager:public event
{
    int budget_id, event_id;
    float total_budget, catering_expense, decor_expense, entertainment_expense, misc_expense;
    float actual_catering, actual_decor, actual_entertainment, actual_misc;

    void track_overspending(float cost, float ideal_cost);

    public:
    BudgetManager(int id, float budget); // Modified constructor
    void track_expenses();
    void print_budget();
};
BudgetManager::BudgetManager(int id, float budget)
{
    event_id = id;
    budget_id = id;
    total_budget = budget;
    catering_expense = 0.4 * total_budget;
    decor_expense = 0.25 * total_budget;
    entertainment_expense = 0.25 * total_budget;
    misc_expense = 0.1 * total_budget;
    actual_catering = actual_decor = actual_entertainment = actual_misc = 0;
}
void BudgetManager::track_expenses()
{
    int check;
    float price;
    cout << "What type of expense do you wanna log?" << endl;
    cout << "Enter 1 for catering \nEnter 2 for Decor \nEnter 3 for Entertainment \nEnter 4 for Miscellaneous" << endl;
    cin >> check;
    cout << "Enter the price you wanna enter" << endl;
    cin >> price;
    switch (check)
    {
    case 1:
        actual_catering += price;
        cout << "Total Catering expense: " << actual_catering << endl;
        track_overspending(actual_catering, catering_expense);
        break;

    case 2:
        actual_decor += price;
        cout << "Total Decor expense: " << actual_decor << endl;
        track_overspending(actual_decor, decor_expense);
        break;

    case 3:
        actual_entertainment += price;
        cout << "Total Entertainment expense: " << actual_entertainment << endl;
        track_overspending(actual_entertainment, entertainment_expense);
        break;

    case 4:
        actual_misc += price;
        cout << "Total Miscellanious expense: " << actual_misc << endl;
        track_overspending(actual_misc, misc_expense);
        break;

    default:
        cout << "Enter a correct number" << endl;
        break;
    }
}
void BudgetManager::track_overspending(float cost, float ideal_cost)
{
    float per = cost / ideal_cost;
    if (per >= 0.85)
    {
        cout << "You have used up " << per*100 << "% of the budget for this category" << endl;
    }
}
void BudgetManager::print_budget()
{
    cout << "For Catering Expense: " << actual_catering << "/" << catering_expense << endl;
    cout << "For Decoration Expense: " << actual_decor << "/" << decor_expense << endl;
    cout << "For Entertainment Expense: " << actual_entertainment << "/" << entertainment_expense << endl;
    cout << "For Miscellanious Expense: " << actual_misc << "/" << misc_expense << endl;
}

class feedback:public event 
{
private:
    string feedback_id, event_id, client_id, client_rating, comment, suggestion;    

public:
    void collect_feedback()
    {    
        cout << "Enter Feedback ID: ";     
        cin >> feedback_id;     
        cout << "Enter Event ID: ";      
        cin >> event_id;    
        cout << "Enter Client ID: ";    
        cin >> client_id;    
        cout << "Enter Client Rating: ";   
        cin >> client_rating;    
        cout << "Enter Comment: ";   
        cin.ignore();
        getline(cin, comment);     
        cout << "Enter Suggestion: ";  
        getline(cin, suggestion);    

        cout << "Feedback Collected from Client: " << client_id << endl;      
    }

    void generate_summary()
    {      
        if (feedback_id.empty()) {
            cout << "No feedback collected yet." << endl;
            return;
        }
        
        cout << "Feedback Summary:" << endl;  
        cout << "Event ID: " << event_id << endl;     
        cout << "Client ID: " << client_id << endl;    
        cout << "Rating: " << client_rating << endl;    
        cout << "Comment: " << comment << endl;    
        cout << "Suggestion: " << suggestion << endl;      
    }
};

int main() 
{
    vector<event> events;
    vector<Schedule> schedules;
    vector<Task> tasks;
    vector<vendor> vendors;
    vector<Client> clients;
    vector<BudgetManager> budgets;
    vector<feedback> feedbacks;
    
    int choice, ch, event_id = 1;
    bool check = true;

    cout << "\nWelcome to the Event Management System!" << endl;
    
    // Display existing events at startup if any
    if(ifstream("events.csv")) 
    {
        event::print_event();
    }

    while(check) 
    {
        cout << "\nSYSTEM MENU" << endl;
        cout << "1. Create New Event" << endl;
        cout << "2. Update Existing Event" << endl;
        cout << "3. Exit System" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                {
                    events.push_back(event(event_id));
                    
                    // Get the budget from the last added event
                    float event_budget = events.back().get_budget();
                    budgets.push_back(BudgetManager(event_id, event_budget));
                    
                    // Add a new schedule, task, and vendor for this event
                    schedules.push_back(Schedule());
                    tasks.push_back(Task());
                    vendors.push_back(vendor());
                    
                    // Create a new client for the event
                    Client new_client;
                    cout << "Do you want to add client details? (1 for yes, 0 for no): ";
                    int add_client;
                    cin >> add_client;
                    if (add_client == 1) 
                    {
                        cin.ignore(); 
                        new_client.addClient();
                    }
                    clients.push_back(new_client);
                    
                    // Create feedback object for the event
                    feedbacks.push_back(feedback());
                    
                    cout << "Event created with ID: " << event_id << endl;
                    event_id++;
                }
                break;
                
            case 2:
                if (events.empty()) 
                {
                    cout << "No existing events to update." << endl;
                } 
                else 
                {
                    int id;
                    bool valid_id = false;
                    
                    while(!valid_id)
                    {
                        event::print_event();
                        cout << "Enter event number to select (1-" << events.size() << "): ";
                        cin >> id;
                        
                        // Check if the number entered is correct
                        if (id > 0 && id <= events.size()) 
                        {
                            id--; // Convert to 0-based index
                            valid_id = true;
                        } 
                        else 
                        {
                            cout << "Invalid event number! Please try again.\n";
                        }
                    }

                    bool event_menu = true;
                    while (event_menu) {
                        cout << "\n\tEVENT MANAGEMENT" << endl;
                        cout << "1. Update event details" << endl;
                        cout << "2. Cancel event" << endl;
                        cout << "3. Manage budget" << endl;
                        cout << "4. Manage Schedule" << endl;
                        cout << "5. Manage Tasks" << endl;
                        cout << "6. Manage Vendors" << endl;
                        cout << "7. Manage Clients" << endl;
                        cout << "8. Manage Feedback" << endl;
                        cout << "9. Return to main menu" << endl;
                        cout << "Enter your choice: ";
                        cin >> ch;

                        switch(ch)
                        {
                            case 1:
                                events[id].update_event();
                                break;
                            case 2:
                                events[id].cancel_event();
                                break;
                            case 3:
                                {
                                    int budget_choice;
                                    cout << "\nBudget Management: " << endl;
                                    cout << "1. Track expenses" << endl;
                                    cout << "2. Print current budget" << endl;
                                    cout << "Enter choice: ";
                                    cin >> budget_choice;

                                    switch(budget_choice) 
                                    {
                                        case 1:
                                            budgets[id].track_expenses();
                                            break;
                                        case 2:
                                            budgets[id].print_budget();
                                            break;
                                        default:
                                            cout << "Invalid choice!" << endl;
                                    }
                                }
                                break;
                            case 4:
                                {
                                    int sched_choice;
                                    cout << "\nSchedule Manager: " << endl;
                                    cout << "1. Create Schedule" << endl;
                                    cout << "2. Update Schedule" << endl;
                                    cout << "3. Print Schedule" << endl;
                                    cout << "Enter choice: ";
                                    cin >> sched_choice;
                                    
                                    switch(sched_choice) 
                                    {
                                        case 1:
                                            schedules[id].create_schedule(id + 1);
                                            break;
                                        case 2:
                                            schedules[id].update_schedule(id + 1);
                                            break;
                                        case 3:
                                            schedules[id].print_schedule();
                                            break;
                                        default:
                                            cout << "Invalid choice!" << endl;
                                    }
                                }
                                break;
                            case 5:
                                {
                                    int task_choice;
                                    cout << "\nTask Manager: " << endl;
                                    cout << "1. Assign task" << endl;
                                    cout << "2. Track task progress" << endl;
                                    cout << "3. Mark task complete" << endl;
                                    cout << "Enter choice: ";
                                    cin >> task_choice;

                                    switch(task_choice) 
                                    {
                                        case 1:
                                            tasks[id].assign_task();
                                            break;
                                        case 2:
                                            tasks[id].track_progress();
                                            break;
                                        case 3:
                                            tasks[id].mark_complete();
                                            break;
                                        default:
                                            cout << "Invalid choice!" << endl;
                                    }
                                }
                                break;
                            case 6:
                                {
                                    int vend_choice;
                                    cout << "\nVendor Manager:" << endl;
                                    cout << "1. Add vendor details" << endl;
                                    cout << "2. View vendor information" << endl;
                                    cout << "3. Confirm vendor service" << endl;
                                    cout << "Enter choice: ";
                                    cin >> vend_choice;

                                    switch(vend_choice) 
                                    {
                                        case 1:
                                            vendors[id].getdata();
                                            break;
                                        case 2:
                                            vendors[id].display_info();
                                            break;
                                        case 3:
                                            vendors[id].confirm_service();
                                            break;
                                        default:
                                            cout << "Invalid choice!" << endl;
                                    }
                                }
                                break;
                            case 7:
                                {
                                    int client_choice;
                                    cout << "\nClient Manager:" << endl;
                                    cout << "1. Add client information" << endl;
                                    cout << "2. View client details" << endl;
                                    cout << "Enter choice: ";
                                    cin >> client_choice;

                                    switch(client_choice) 
                                    {
                                        case 1:
                                            cin.ignore();
                                            clients[id].addClient();
                                            break;
                                        case 2:
                                            clients[id].displayClient();
                                            break;
                                        default:
                                            cout << "Invalid choice!" << endl;
                                    }
                                }
                                break;
                            case 8:
                                {
                                    int feedback_choice;
                                    cout << "\nFeedback Manager:" << endl;
                                    cout << "1. Collect feedback" << endl;
                                    cout << "2. View feedback summary" << endl;
                                    cout << "Enter choice: ";
                                    cin >> feedback_choice;

                                    switch(feedback_choice) 
                                    {
                                        case 1:
                                            feedbacks[id].collect_feedback();
                                            break;
                                        case 2:
                                            feedbacks[id].generate_summary();
                                            break;
                                        default:
                                            cout << "Invalid choice!" << endl;
                                    }
                                }
                                break;
                            case 9:
                                event_menu = false;
                                break;
                            default:
                                cout << "Invalid choice! Please try again." << endl;
                        }
                    }
                }
                break;
            case 3:
                cout << "Exiting Event Management System. Goodbye!" << endl;
                check = false;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
    
    return 0;
}