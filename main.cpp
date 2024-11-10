#include <iostream>
#include "games.h"        // Đưa vào các hàm từ file games.h
#include "leaderboard.h"  // Đưa vào các hàm từ file leaderboard.h

using namespace std;

int main()
{
    int main_choice;
    // Vòng lặp chính của menu trò chơi
    while (true)
    {
        cout << "\nMain Menu:\n";  // In menu chính
        cout << "\t1. Play Tower of Hanoi\n";  // Lựa chọn chơi trò chơi Tower of Hanoi
        cout << "\t2. Leaderboard\n";  // Lựa chọn xem bảng xếp hạng
        cout << "\t3. Exit\n";  // Lựa chọn thoát chương trình
        cout << "Enter your choice: ";
        cin >> main_choice;  // Nhập lựa chọn từ người dùng

        // Thực hiện theo lựa chọn của người dùng
        switch (main_choice)
        {
        case 1:
            playGame();  // Gọi hàm chơi game từ games.h
            break;
        case 2:
            leaderboardMenu();  // Gọi hàm menu bảng xếp hạng từ leaderboard.h
            break;
        case 3:
            cout << "Exiting program.\n";  // In thông báo thoát
            return 0;  // Kết thúc chương trình
        default:
            cout << "Invalid choice, try again.\n";  // Thông báo nếu nhập không hợp lệ
        }
    }
}
