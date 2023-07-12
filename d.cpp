#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

class UserProfile
{
private:
    string username;
    vector<string> friendRequests;
    vector<string> pendingRequests;
    vector<string> friends;
    vector<string> posts;
    vector<int> postLikes;

public:
    UserProfile(const string &name)
    {
        username = name;
    }

    string getUsername() const
    {
        return username;
    }

    void addFriendRequest(const string &username)
    {
        friendRequests.push_back(username);
    }

    void addPendingRequest(const string &username)
    {
        pendingRequests.push_back(username);
    }

    const vector<string> &getPendingRequests() const
    {
        return pendingRequests;
    }

    void removeFriendRequest(const string &username)
    {
        for (auto it = friendRequests.begin(); it != friendRequests.end(); ++it)
        {
            if (*it == username)
            {
                friendRequests.erase(it);
                break;
            }
        }
    }

    void removePendingRequest(const string &username)
    {
        for (auto it = pendingRequests.begin(); it != pendingRequests.end(); ++it)
        {
            if (*it == username)
            {
                pendingRequests.erase(it);
                break;
            }
        }
    }

    bool isFriend(const string &username) const
    {
        for (const string &friendUsername : friends)
        {
            if (friendUsername == username)
            {
                return true;
            }
        }
        return false;
    }

    bool hasFriendRequestFrom(const string &username) const
    {
        for (const string &requestUsername : friendRequests)
        {
            if (requestUsername == username)
            {
                return true;
            }
        }
        return false;
    }

    bool hasPendingRequestFrom(const string &username) const
    {
        for (const string &requestUsername : pendingRequests)
        {
            if (requestUsername == username)
            {
                return true;
            }
        }
        return false;
    }

    void showPendingRequests() const
    {
        if (pendingRequests.empty())
        {
            cout << "\t\tNo pending friend requests." << endl;
        }
        else
        {
            cout << "\t\tPending Friend Requests:" << endl;
            for (const string &requestUsername : pendingRequests)
            {
                cout << "\t\t- " << requestUsername << endl;
            }
        }
    }

    void showFriendList() const
    {
        if (friends.empty())
        {
            cout << "\t\tNo friends in the friend list." << endl;
        }
        else
        {
            cout << "\t\tFriend List:" << endl;
            for (const string &friendUsername : friends)
            {
                cout << "\t\t- " << friendUsername << endl;
            }
        }
    }

    vector<string> getFriendList() const
    {
        return friends;
    }

    void addFriend(const string &username)
    {
        friends.push_back(username);
        removeFriendRequest(username);
        removePendingRequest(username);
    }

    void removeFriend(const string &username)
    {
        for (auto it = friends.begin(); it != friends.end(); ++it)
        {
            if (*it == username)
            {
                friends.erase(it);
                return;
            }
        }
    }

    void addPost(const string &post)
    {
        posts.push_back(post);
        postLikes.push_back(0);
    }

    void deletePost(int index)
    {
        if (index >= 0 && index < posts.size())
        {
            posts.erase(posts.begin() + index);
            postLikes.erase(postLikes.begin() + index);
        }
    }

    const vector<string> &getPosts() const
    {
        return posts;
    }

    void showPosts() const
    {
        if (posts.empty())
        {
            cout << "\t\tNo posts available." << endl;
        }
        else
        {
            cout << "\t\tPosts:" << endl;
            for (size_t i = 0; i < posts.size(); ++i)
            {
                cout << "\t\t- [" << i << "] " << posts[i] << " (Likes: " << postLikes[i] << ")" << endl;
            }
        }
    }

    void likePost(int index)
    {
        if (index >= 0 && index < postLikes.size())
        {
            postLikes[index]++;
        }
    }

    bool canSeePosts(const string &username) const
    {
        return isFriend(username) || username == this->username;
    }

    bool canLikePosts(const string &username) const
    {
        return isFriend(username);
    }
};

class User
{
private:
    string username;
    string password;
    UserProfile *profile;

public:
    User(const string &name, const string &pass)
    {
        username = name;
        password = pass;
        profile = nullptr;
    }

    string getUsername() const
    {
        return username;
    }

    string getPassword() const
    {
        return password;
    }

    UserProfile *getProfile() const
    {
        return profile;
    }

    void createProfile()
    {
        profile = new UserProfile(username);
    }
};

class UserManager
{
private:
    vector<User> users;
    string filename;
    UserProfile *currentUser;

public:
    UserManager(const string &file)
    {
        filename = file;
        currentUser = nullptr;
        loadUsers();
    }

    ~UserManager()
    {
        saveUsers();
        for (User &user : users)
        {
            delete user.getProfile();
        }
    }

    void saveUsers()
    {
        ofstream file(filename);

        if (file.is_open())
        {
            for (const User &user : users)
            {
                file << user.getUsername() << " " << user.getPassword() << endl;
            }

            file.close();
        }
        else
        {
            cout << "\t\tUnable to save users. File not found." << endl;
        }
    }

    void loadUsers()
    {
        ifstream file(filename);
        users.clear();

        if (file.is_open())
        {
            string username, password;

            while (file >> username >> password)
            {
                User newUser(username, password);
                newUser.createProfile();

                users.push_back(newUser);
            }

            file.close();
        }
    }

    void registerUser()
    {
        string username, password;
        cout << "\t\tEnter User Name: ";
        cin >> username;
        cout << "\t\tEnter Password: ";
        cin >> password;

        User newUser(username, password);
        newUser.createProfile();

        users.push_back(newUser);
        saveUsers();

        cout << "\t\tUser Registered Successfully." << endl;
    }

    void loginUser(const string &name, const string &pass)
    {
        for (User &user : users)
        {
            if (user.getUsername() == name && user.getPassword() == pass)
            {
                cout << "\t\tLogin Successful." << endl;
                UserProfile *profile = user.getProfile();
                if (profile != nullptr)
                {
                    currentUser = profile;
                    profile->showPendingRequests();
                    profileMenu(currentUser);
                }
                else
                {
                    cout << "\t\tUser profile not found." << endl;
                }
                return;
            }
        }
        cout << "\t\tInvalid User Name or Password." << endl;
    }

    User *findUserByUsername(const string &username) const
    {
        for (const User &user : users)
        {
            if (user.getUsername() == username)
            {
                return const_cast<User *>(&user);
                ;
            }
        }
        return nullptr;
    }

    void showUsers() const
    {
        cout << "\t\t--- Users List ---" << endl;
        for (const User &user : users)
        {
            cout << "\t\t" << user.getUsername() << endl;
        }
    }

    void searchUser(const string &username) const
    {
        User *user = findUserByUsername(username);
        if (user != nullptr)
        {
            cout << "\t\tUser Found" << endl;
        }
        else
        {
            cout << "\t\tUser Not Found" << endl;
        }
    }

    void deleteUser(const string &username)
    {
        for (User &user : users)
        {
            UserProfile *profile = user.getProfile();
            if (profile != nullptr)
            {
                profile->removeFriend(username);
                profile->removeFriendRequest(username);
                profile->removePendingRequest(username);
            }
        }

        for (auto it = users.begin(); it != users.end(); ++it)
        {
            if (it->getUsername() == username)
            {
                delete it->getProfile();
                users.erase(it);
                saveUsers();
                cout << "\t\tUser Removed Successfully." << endl;
                return;
            }
        }
        cout << "\t\tUser Not Found." << endl;
    }

    void sendFriendRequest(const string &sender, const string &receiver)
    {
        User *senderUser = findUserByUsername(sender);
        User *receiverUser = findUserByUsername(receiver);

        if (senderUser != nullptr && receiverUser != nullptr)
        {
            UserProfile *senderProfile = senderUser->getProfile();
            UserProfile *receiverProfile = receiverUser->getProfile();

            senderProfile->addFriendRequest(receiver);
            receiverProfile->addPendingRequest(sender);

            cout << "\t\tFriend Request Sent Successfully." << endl;
        }
        else
        {
            cout << "\t\tInvalid Usernames. Please Try Again." << endl;
        }
    }

    void acceptFriendRequest(const string &username, const string &friendUsername)
    {
        User *user = findUserByUsername(username);
        User *friendUser = findUserByUsername(friendUsername);

        if (user != nullptr && friendUser != nullptr)
        {
            UserProfile *profile = user->getProfile();
            UserProfile *friendProfile = friendUser->getProfile();

            if (friendProfile->hasFriendRequestFrom(username))
            {
                profile->addFriend(friendUsername);
                friendProfile->addFriend(username);

                profile->removePendingRequest(friendUsername);
                friendProfile->removeFriendRequest(username);

                cout << "\t\tFriend Request Accepted Successfully." << endl;
            }
            else
            {
                cout << "\t\tNo pending request from that particular user." << endl;
            }
        }
        else
        {
            cout << "\t\tInvalid Usernames. Please Try Again." << endl;
        }
    }

    void manageFriendRequests()
    {
        int option;
        char choice;

        do
        {
            system("cls");
            cout << "\n\n\t\t--- Friend Requests Menu ---" << endl;
            cout << "\t\t1. Show Pending Requests" << endl;
            cout << "\t\t2. Accept Friend Request" << endl;
            cout << "\t\t3. Go Back" << endl;
            cout << "\t\tEnter Your Choice: ";
            cin >> option;

            switch (option)
            {
            case 1:
            {
                currentUser->showPendingRequests();
                break;
            }
            case 2:
            {
                string friendUsername;
                cout << "\t\tEnter Friend's Username: ";
                cin >> friendUsername;
                acceptFriendRequest(currentUser->getUsername(), friendUsername);
                break;
            }
            case 3:
                return;
            default:
                cout << "\t\tInvalid Option. Please Try Again." << endl;
                break;
            }

            cout << "\t\tDo You Want to Continue [Yes/No]? ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }

    void profileMenu(UserProfile *profile)
    {
        int option;
        char choice;

        do
        {
            system("cls");
            string username = profile->getUsername();
            cout << "\t\tThis is " << username << "'s profile." << endl;
            cout << "\n\n\t\t--- Profile Menu ---" << endl;
            cout << "\t\t1. Manage Friend Requests" << endl;
            cout << "\t\t2. Send Friend Request" << endl;
            cout << "\t\t3. View Friend List" << endl;
            cout << "\t\t4. Add Post" << endl;
            cout << "\t\t5. Delete Post" << endl;
            cout << "\t\t6. Show Posts" << endl;
            cout << "\t\t7. Like Post" << endl;
            cout << "\t\t8. Show Posts of your friends" << endl;
            cout << "\t\t9. Like Posts of your friends" << endl;
            cout << "\t\t10. Logout" << endl;
            cout << "\t\tEnter Your Choice: ";
            cin >> option;

            switch (option)
            {
            case 1:
            {
                manageFriendRequests();
                break;
            }
            case 2:
            {
                string friendUsername;
                cout << "\t\tEnter Friend's Username: ";
                cin >> friendUsername;
                sendFriendRequest(currentUser->getUsername(), friendUsername);
                break;
            }
            case 3:
            {
                currentUser->showFriendList();
                break;
            }
            case 4:
            {
                string post;
                cout << "\t\tEnter your post: ";
                cin.ignore();
                getline(cin, post);
                profile->addPost(post);
                cout << "\t\tPost added successfully!" << endl;
                break;
            }
            case 5:
            {
                profile->showPosts();
                if (profile->getPosts().empty())
                {
                    cout << "\t\tNo posts available to delete." << endl;
                    break;
                }
                int postIndex;
                cout << "\t\tEnter the index of the post you want to delete: ";
                cin >> postIndex;
                profile->deletePost(postIndex);
                cout << "\t\tPost deleted successfully!" << endl;
                break;
            }
            case 6:
            {
                profile->showPosts();
                break;
            }
            case 7:
            {
                profile->showPosts();
                if (profile->getPosts().empty())
                {
                    cout << "\t\tNo posts available to like." << endl;
                    break;
                }
                int postIndex;
                cout << "\t\tEnter the index of the post you want to like: ";
                cin >> postIndex;
                profile->likePost(postIndex);
                cout << "\t\tPost liked successfully!" << endl;
                break;
            }
            case 8:
            {
                showPostsOfFriends(profile->getFriendList());
                break;
            }
            case 9:
            {
                if (profile->getFriendList().empty())
                {
                    cout << "\t\tYou have no friends to like their posts." << endl;
                    break;
                }
                int friendIndex;
                cout << "\t\tEnter the index of the friend whose posts you want to like: ";
                cin >> friendIndex;
                likePostsOfFriend(profile->getFriendList(), friendIndex);
                break;
            }
            case 10:
            {
                cout << "\t\tLogging out..." << endl;
                currentUser = nullptr;
                return;
            }
            default:
                cout << "\t\tInvalid Option. Please Try Again." << endl;
                break;
            }

            cout << "\t\tDo You Want to Continue [Yes/No]? ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }

    void showPostsOfFriends(const vector<string> &friendList) const
    {
        if (friendList.empty())
        {
            cout << "\t\tYou have no friends to see their posts." << endl;
        }
        else
        {
            cout << "\t\t--- Posts of your friends ---" << endl;
            for (const string &friendUsername : friendList)
            {
                User *friendUser = findUserByUsername(friendUsername);
                if (friendUser != nullptr)
                {
                    UserProfile *friendProfile = friendUser->getProfile();
                    if (friendProfile != nullptr && friendProfile->canSeePosts(currentUser->getUsername()))
                    {
                        cout << "\n\t\t--- " << friendUsername << "'s Posts ---" << endl;
                        friendProfile->showPosts();
                    }
                }
            }
        }
    }

    void likePostsOfFriend(const vector<string> &friendList, int friendIndex)
    {
        if (friendIndex >= 0 && friendIndex < friendList.size())
        {
            const string &friendUsername = friendList[friendIndex];
            User *friendUser = findUserByUsername(friendUsername);
            if (friendUser != nullptr)
            {
                UserProfile *friendProfile = friendUser->getProfile();
                if (friendProfile != nullptr && friendProfile->canLikePosts(currentUser->getUsername()))
                {
                    friendProfile->showPosts();
                    if (friendProfile->getPosts().empty())
                    {
                        cout << "\t\tNo posts available to like." << endl;
                    }
                    else
                    {
                        int postIndex;
                        cout << "\t\tEnter the index of the post you want to like: ";
                        cin >> postIndex;
                        friendProfile->likePost(postIndex);
                        cout << "\t\tPost liked successfully!" << endl;
                    }
                }
                else
                {
                    cout << "\t\tYou are not allowed to like posts of that friend." << endl;
                }
            }
        }
        else
        {
            cout << "\t\tInvalid friend index." << endl;
        }
    }

    void start()
    {
        int option;
        char choice;

        do
        {
            system("cls");
            cout << "\n\n\t\t--- Welcome to social network connecter app ---" << endl;
            cout << "\t\t1. Register new User" << endl;
            cout << "\t\t2. Login existing User" << endl;
            cout << "\t\t3. Search User" << endl;
            cout << "\t\t4. Show All Users" << endl;
            cout << "\t\t5. Delete User" << endl;
            cout << "\t\t6. Exit" << endl;
            cout << "\t\tEnter Your Choice: ";
            cin >> option;

            switch (option)
            {
            case 1:
                registerUser();
                break;
            case 2:
            {
                string username, password;
                cout << "\t\tEnter User Name: ";
                cin >> username;
                cout << "\t\tEnter Password: ";
                cin >> password;
                loginUser(username, password);
                break;
            }
            case 3:
            {
                string username;
                cout << "\t\tEnter User Name to Search: ";
                cin >> username;
                searchUser(username);
                break;
            }
            case 4:
                showUsers();
                break;
            case 5:
            {
                string username;
                cout << "\t\tEnter User Name to Delete: ";
                cin >> username;
                deleteUser(username);
                break;
            }
            case 6:
                cout << "\t\tExiting..." << endl;
                return;
            default:
                cout << "\t\tInvalid Option. Please Try Again." << endl;
                break;
            }

            cout << "\t\tDo You Want to Continue [Yes/No]? ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }
};

int main()
{
    UserManager userManager("users.txt");
    userManager.start();

    return 0;
}
