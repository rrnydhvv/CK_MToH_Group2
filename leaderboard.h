#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Cấu trúc lưu trữ kết quả trò chơi
struct GameResult
{
    string player_name;      // Tên người chơi
    string game_mode;        // Chế độ trò chơi (Standard hoặc Magnetic)
    string difficulty;       // Độ khó của trò chơi (Easy, Normal, Hard, Custom)
    int num_disk;            // Số lượng đĩa trong trò chơi
    int moves;               // Số bước di chuyển để hoàn thành trò chơi
    double time_taken;       // Thời gian hoàn thành trò chơi (tính bằng giây)

    // Hàm khởi tạo cho GameResult
    GameResult(string player_name, string game_mode, string difficulty, int num_disk, int moves, double time_taken)
        : player_name(player_name), game_mode(game_mode), difficulty(difficulty), num_disk(num_disk), moves(moves), time_taken(time_taken) {}
};

// Mảng chứa danh sách các kết quả trò chơi
vector<GameResult> leaderboard;

// Hàm tải dữ liệu từ file game_results.txt vào mảng leaderboard
void loadLeaderboardFromFile()
{
    ifstream file("game_results.txt");  // Mở file để đọc
    string line;
    leaderboard.clear();  // Xóa tất cả kết quả cũ trong leaderboard

    // Đọc từng dòng trong file và thêm vào leaderboard
    while (getline(file, line))
    {
        istringstream ss(line);  // Chia dòng thành các phần
        string player_name, game_mode, difficulty;
        int num_disk, moves;
        double time_taken;

        ss >> player_name >> game_mode >> difficulty >> num_disk >> moves >> time_taken;

        leaderboard.push_back(GameResult(player_name, game_mode, difficulty, num_disk, moves, time_taken));  // Thêm kết quả vào leaderboard
    }
}

// Hàm hiển thị bảng xếp hạng (leaderboard)
void showLeaderboard()
{
    cout << "\nLeaderboard:\n";
    if (leaderboard.empty())  // Nếu không có kết quả nào
    {
        cout << "No game results yet.\n";
        return;
    }

    // In tiêu đề bảng xếp hạng
    cout << left << setw(20) << "Player Name"
         << setw(15) << "Game Mode"
         << setw(12) << "Difficulty"
         << setw(7)  << "Disks"
         << setw(7)  << "Moves"
         << setw(15) << "Time (seconds)" << endl;

    cout << "---------------------------------------------------------------------------\n";

    // In các kết quả trong bảng xếp hạng
    for (const auto& result : leaderboard)
    {
        cout << left << setw(20) << result.player_name
             << setw(15) << result.game_mode
             << setw(12) << result.difficulty
             << setw(7)  << result.num_disk
             << setw(7)  << result.moves
             << fixed << setprecision(3) << setw(15) << result.time_taken << endl;
    }
}

// Hàm áp dụng bộ lọc theo tên, chế độ trò chơi hoặc độ khó
void applyFilter(string filter_type, string filter_value)
{
    vector<GameResult> filtered;

    // Lọc theo điều kiện đã nhập
    for (const auto& result : leaderboard)
    {
        if (filter_type == "name" && result.player_name.find(filter_value) != string::npos)
            filtered.push_back(result);
        else if (filter_type == "game_mode" && result.game_mode.find(filter_value) != string::npos)
            filtered.push_back(result);
        else if (filter_type == "difficulty" && result.difficulty.find(filter_value) != string::npos)
            filtered.push_back(result);
    }

    leaderboard = filtered;  // Cập nhật lại leaderboard với kết quả đã lọc
}

// Hàm xóa bộ lọc, hiển thị lại tất cả kết quả
void removeFilter()
{
    loadLeaderboardFromFile();  // Tải lại tất cả dữ liệu từ file
}

// Hàm so sánh các kết quả theo số bước di chuyển (tăng dần)
bool compareByMovesAscending(const GameResult& a, const GameResult& b)
{
    return a.moves < b.moves;
}

// Hàm so sánh các kết quả theo số bước di chuyển (giảm dần)
bool compareByMovesDescending(const GameResult& a, const GameResult& b)
{
    return a.moves > b.moves;
}

// Hàm so sánh các kết quả theo thời gian (tăng dần)
bool compareByTimeAscending(const GameResult& a, const GameResult& b)
{
    return a.time_taken < b.time_taken;
}

// Hàm so sánh các kết quả theo thời gian (giảm dần)
bool compareByTimeDescending(const GameResult& a, const GameResult& b)
{
    return a.time_taken > b.time_taken;
}

// Hàm sắp xếp bảng xếp hạng theo số bước di chuyển hoặc thời gian, theo thứ tự tăng dần hoặc giảm dần
void sortLeaderboard(int sort_choice, int priority_choice)
{
    if (priority_choice == 1)  // Sắp xếp theo số bước di chuyển
    {
        if (sort_choice == 1)  // Tăng dần
            sort(leaderboard.begin(), leaderboard.end(), compareByMovesAscending);
        else  // Giảm dần
            sort(leaderboard.begin(), leaderboard.end(), compareByMovesDescending);
    }
    else if (priority_choice == 2)  // Sắp xếp theo thời gian
    {
        if (sort_choice == 1)  // Tăng dần
            sort(leaderboard.begin(), leaderboard.end(), compareByTimeAscending);
        else  // Giảm dần
            sort(leaderboard.begin(), leaderboard.end(), compareByTimeDescending);
    }
}

// Hàm quản lý menu bảng xếp hạng (leaderboard)
void leaderboardMenu()
{
    int choice;
    bool exit_menu = false;
    string filter_type, filter_value;
    int sort_choice, priority_choice;
    loadLeaderboardFromFile();  // Tải bảng xếp hạng từ file

    // Vòng lặp để hiển thị menu và xử lý các lựa chọn
    while (!exit_menu)
    {
        cout << "\nLeaderboard Menu:\n";
        cout << "\t1. Show Leaderboard\n";
        cout << "\t2. Filter Data\n";
        cout << "\t3. Remove Filter\n";
        cout << "\t4. Sort Leaderboard\n";
        cout << "\t5. Exit to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            showLeaderboard();  // Hiển thị bảng xếp hạng
            break;

        case 2:
            // Lọc dữ liệu theo tên, chế độ trò chơi hoặc độ khó
            cout << "Enter filter type (name, game_mode, difficulty): ";
            cin >> filter_type;
            cout << "Enter filter value: ";
            cin >> filter_value;
            applyFilter(filter_type, filter_value);
            break;

        case 3:
            removeFilter();  // Xóa bộ lọc
            cout << "Filter removed. Showing all results.\n";
            break;

        case 4:
            // Sắp xếp bảng xếp hạng theo số bước di chuyển hoặc thời gian
            cout << "Sort by:\n1. Moves\n2. Time\nEnter your choice: ";
            cin >> priority_choice;
            cout << "Sort order:\n1. Ascending\n2. Descending\nEnter your choice: ";
            cin >> sort_choice;
            sortLeaderboard(sort_choice, priority_choice);
            cout << "Leaderboard sorted.\n";
            break;

        case 5:
            exit_menu = true;  // Thoát menu
            break;

        default:
            cout << "Invalid choice, try again.\n";  // Nếu nhập lựa chọn không hợp lệ
        }
    }
}

#endif // LEADERBOARD_H_INCLUDED
