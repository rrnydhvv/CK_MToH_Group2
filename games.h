#ifndef GAMES_H_INCLUDED
#define GAMES_H_INCLUDED

#include <iostream>
#include <fstream>
#include <ctime>
#include <stack>

using namespace std;

#define CYAN "\033[36m"  // Màu xanh
#define RED "\033[31m"   // Màu đỏ
#define RESET "\033[37m" // Đặt lại màu sắc mặc định

// Cấu trúc đại diện cho một đĩa trong trò chơi
struct Disk
{
    char label;  // Nhãn của đĩa (ví dụ: 'A', 'B', 'C', ...)
    int isRed;   // 0: Màu xanh, 1: Màu đỏ, 2: Đĩa không có màu

    // Hàm khởi tạo cho Disk
    Disk(char Label, int IsRed)
    {
        label = Label;
        isRed = IsRed;
    }

    // Hàm in ra đĩa với màu sắc tương ứng
    void print()
    {
        if (isRed == 1)
            cout << RED << label << RESET;  // In đĩa màu đỏ
        if (isRed == 0)
            cout << CYAN << label << RESET; // In đĩa màu xanh
        if (isRed == 2)
            cout << label;  // In đĩa không có màu
    }
};

// Cấu trúc đại diện cho trò chơi
struct Game
{
    stack<Disk> str, tmp, dst;  // Ba cột: str, tmp, dst
    int game_mode, num_disk;     // Chế độ trò chơi và số lượng đĩa

    // Hàm khởi tạo cho Game
    Game(int gm, int nd)
    {
        game_mode = gm;
        num_disk = nd;
        initGame();  // Khởi tạo trò chơi
    }

    // Hàm khởi tạo các cột và đĩa trong trò chơi
    void initGame()
    {
        int isRed;
        if (game_mode == 1)  // Chế độ trò chơi "Standard"
        {
            isRed = 2;  // Không có màu cho các đĩa trong chế độ này
            for (int i = num_disk; i > 0; --i)
            {
                Disk newDisk('A' + i - 1, 2);  // Tạo đĩa mới và thêm vào cột str
                str.push(newDisk);
            }
        }
        else  // Chế độ trò chơi "Magnetic"
        {
            isRed = 1;  // Bắt đầu với đĩa đỏ
            for (int i = num_disk; i > 0; --i)
            {
                Disk newDisk('A' + i - 1, isRed % 2);  // Tạo đĩa với màu sắc xen kẽ (đỏ, xanh)
                str.push(newDisk);
                ++isRed;  // Chuyển màu sắc cho đĩa tiếp theo
            }
        }
    }

    // Hàm di chuyển đĩa từ cột này sang cột khác
    bool moveDisk(int fromColumn, int toColumn)
    {
        stack<Disk>* fromStack;
        stack<Disk>* toStack;

        // Xác định cột xuất phát
        switch (fromColumn)
        {
        case 1:
            fromStack = &str;
            break;
        case 2:
            fromStack = &tmp;
            break;
        case 3:
            fromStack = &dst;
            break;
        default:
            cout << "Invalid column!" << endl;
            return false;
        }

        // Xác định cột đích
        switch (toColumn)
        {
        case 1:
            toStack = &str;
            break;
        case 2:
            toStack = &tmp;
            break;
        case 3:
            toStack = &dst;
            break;
        default:
            cout << "Invalid column!" << endl;
            return false;
        }

        // Kiểm tra xem cột xuất phát có đĩa hay không
        if (fromStack->empty())
        {
            cout << "No disk to move in the source column!" << endl;
            return false;
        }

        Disk diskToMove = fromStack->top();  // Lấy đĩa cần di chuyển
        if (toStack->empty())  // Nếu cột đích rỗng, chỉ cần di chuyển đĩa
        {
            fromStack->pop();
            toStack->push(diskToMove);
            return true;
        }

        // Kiểm tra xem đĩa có thể di chuyển được không (theo kích thước và màu sắc)
        bool isValidSize = toStack->top().label > diskToMove.label;
        bool isValidColor = (game_mode == 1 || (game_mode == 2 && toStack->top().isRed != diskToMove.isRed));

        if (isValidSize && isValidColor)
        {
            fromStack->pop();
            toStack->push(diskToMove);
            return true;
        }

        cout << "Invalid move!" << endl;  // Nếu không hợp lệ
        return false;
    }

    // Hàm hiển thị trạng thái của trò chơi (các cột)
    void displayState()
    {
        cout << "Source Column (str): ";
        printColumn(str);  // In cột str
        cout << "Auxiliary Column (tmp): ";
        printColumn(tmp);  // In cột tmp
        cout << "Destination Column (dst): ";
        printColumn(dst);  // In cột dst
    }

    // Hàm in cột
    void printColumn(stack<Disk>& column)
    {
        if (column.empty())
        {
            cout << "Empty" << endl;
            return;
        }
        stack<Disk> temp = column;
        while (!temp.empty())
        {
            temp.top().print();  // In từng đĩa trong cột
            cout << " ";
            temp.pop();
        }
        cout << endl;
    }

    // Kiểm tra xem trò chơi đã hoàn thành chưa
    bool isGameComplete()
    {
        return dst.size() == num_disk;  // Trò chơi hoàn thành khi tất cả đĩa chuyển sang cột đích
    }
};

// Hàm lưu kết quả trò chơi vào file
void saveGameResult(const string& player_name, int game_mode, int diff, int num_disk, int moveCnt, double time_taken)
{
    ofstream outFile("game_results.txt", ios::app);  // Mở file để thêm dữ liệu mới vào
    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    outFile << player_name << " "
            << (game_mode == 1 ? "Standard" : "Magnetic") << " "
            << (diff == 1 ? "Easy" : (diff == 2 ? "Normal" : (diff == 3 ? "Hard" : "Custom"))) << " "
            << num_disk << " "
            << moveCnt << " "
            << time_taken << endl;  // Ghi kết quả vào file
    outFile.close();
}

// Hàm bắt đầu trò chơi
void playGame()
{
    string player_name;
    int game_mode, diff, num_disk;

    cout << "Enter player's name: ";
    cin >> player_name;

    // Lựa chọn chế độ trò chơi
    cout << "Choose your game's mode:\n\t";
    cout << "1. Standard Tower of Ha Noi\n\t";
    cout << "2. Magnetic Tower of Ha Noi\n";
    cout << "Enter game's mode: ";
    cin >> game_mode;

    // Lựa chọn độ khó của trò chơi
    cout << "Choose your game's difficulty:\n\t";
    cout << "1. Easy (3 Disks)\n\t";
    cout << "2. Normal (5 Disks)\n\t";
    cout << "3. Hard (7 Disks)\n\t";
    cout << "4. Custom\n";
    cout << "Enter game's difficulty: ";
    cin >> diff;

    if (diff == 1)
        num_disk = 3;
    if (diff == 2)
        num_disk = 5;
    if (diff == 3)
        num_disk = 7;
    if (diff == 4)
    {
        cout << "Enter your number of disks (1 - 26): ";
        cin >> num_disk;
    }

    cout << "\nPlayer: " << player_name << endl;
    cout << "Game Mode: " << (game_mode == 1 ? "Standard" : "Magnetic") << endl;
    cout << "Difficulty: ";
    if (diff == 1)
        cout << "Easy (3 Disks)";
    else if (diff == 2)
        cout << "Normal (5 Disks)";
    else if (diff == 3)
        cout << "Hard (7 Disks)";
    else
        cout << "Custom (" << num_disk << " Disks)";
    cout << endl
         << endl;

    Game G(game_mode, num_disk);  // Khởi tạo trò chơi
    G.displayState();

    int fromColumn, toColumn;
    int moveCnt = 0;
    clock_t start_time = clock();

    // Vòng lặp chơi cho đến khi hoàn thành
    while (!G.isGameComplete())
    {
        cout << "\nEnter the column to move from (1-3) and to (1-3): ";
        cin >> fromColumn >> toColumn;
        if (G.moveDisk(fromColumn, toColumn))
        {
            cout << "Move successful!" << endl;
            ++moveCnt;
            G.displayState();
        }
        else
            cout << "Move failed! Try again." << endl;
    }

    clock_t end_time = clock();
    double time_taken = double(end_time - start_time) / CLOCKS_PER_SEC;

    // Lưu kết quả trò chơi
    saveGameResult(player_name, game_mode, diff, num_disk, moveCnt, time_taken);

}

#endif // GAMES_H_INCLUDED
