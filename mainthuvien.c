#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int id;
    char title[100];
    char author[100];
    int count;
} Book;

typedef struct {
    int id;
    char name[100];
} Borrower;

typedef struct{
    int id;
    int idBook;
    int idName;
    int ngay, thang, nam;
    int available;
} Phieu;

void writeBooks(char filename[], int n, Book *book) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    fprintf(f, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d\n%s\n%s\n%d\n", book[i].id, book[i].title, book[i].author, book[i].count);
    }
    fclose(f);
}

void writeBorrowers(char filename[], int m, Borrower *borrower) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    fprintf(f, "%d\n", m);
    for (int i = 0; i < m; i++) {
        fprintf(f, "%d\n%s\n", borrower[i].id, borrower[i].name);
    }
    fclose(f);
}

int bookExists(int id, int n, Book *book) {
    for (int i = 0; i < n; i++) {
        if (book[i].id == id) {
            return 1;  
        }
    }
    return 0;  
}

int borrowerExists(int id, int m, Borrower *borrower) {
    for (int i = 0; i < m; i++) {
        if (borrower[i].id == id) {
            return 1;  
        }
    }
    return 0;  
}

void inputBook(char filename[], int *n, Book **book) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    fscanf(f, "%d\n", n);
    *book = (Book *)malloc(sizeof(Book) * *n);
    for (int i = 0; i < *n; i++) {
        fscanf(f, "%d\n", &(*book)[i].id);
        fgets((*book)[i].title, 100, f);
        (*book)[i].title[strcspn((*book)[i].title, "\n")] = 0;
        fgets((*book)[i].author, 100, f);
        (*book)[i].author[strcspn((*book)[i].author, "\n")] = 0;
        fscanf(f, "%d\n", &(*book)[i].count);
    }
    fclose(f);
}

void inputBorrower(char filename[], int *m, Borrower **borrower) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    fscanf(f, "%d\n", m);
    *borrower = (Borrower *)malloc(sizeof(Borrower) * *m);
    for (int i = 0; i < *m; i++) {
        fscanf(f, "%d\n", &(*borrower)[i].id);
        fgets((*borrower)[i].name, 100, f);
        (*borrower)[i].name[strcspn((*borrower)[i].name, "\n")] = 0;
    }
    fclose(f);
}

void addBook(int *n, Book **book, char filename[]) {
    int newId;
    printf("\nNhap ma sach: ");
    scanf("%d", &newId);
    getchar();

    if (bookExists(newId, *n, *book)) {
        printf("Ma sach %d da ton tai. Khong the them moi.\n", newId);
        return;
    }

    *book = realloc(*book, sizeof(Book) * (*n + 1));
    Book *newBook = &(*book)[*n];
    
    newBook->id = newId;
    
    printf("\nNhap ten sach: ");
    fgets(newBook->title, 100, stdin);
    newBook->title[strcspn(newBook->title, "\n")] = 0;

    printf("\nNhap tac gia: ");
    fgets(newBook->author, 100, stdin);
    newBook->author[strcspn(newBook->author, "\n")] = 0;

    printf("\nNhap so luong sach: ");
    scanf("%d", &newBook->count);

    (*n)++;
    writeBooks(filename, *n, *book);
    printf("Da them sach moi thanh cong.\n");
}

void editBook(int idBook, int n, Book *book, char filename[]) {
    for (int i = 0; i < n; i++) {
        if (book[i].id == idBook) {
            printf("\nSua ten sach (de giu nguyen an Enter): ");
            char newTitle[100];
            fgets(newTitle, sizeof(newTitle), stdin);
            newTitle[strcspn(newTitle, "\n")] = '\0';
            if (strlen(newTitle) > 0) {
                strcpy(book[i].title, newTitle);
            }

            printf("\nSua tac gia (de giu nguyen an Enter): ");
            char newAuthor[100];
            fgets(newAuthor, sizeof(newAuthor), stdin);
            newAuthor[strcspn(newAuthor, "\n")] = '\0';
            if (strlen(newAuthor) > 0) {
                strcpy(book[i].author, newAuthor);
            }

            printf("\nSua so luong sach (de giu nguyen an Enter): ");
            char inputBuffer[100];
            fgets(inputBuffer, sizeof(inputBuffer), stdin);
            inputBuffer[strcspn(inputBuffer, "\n")] = '\0';
            if (strlen(inputBuffer) > 0) {
                int newCount;
                if (sscanf(inputBuffer, "%d", &newCount) == 1) {
                    book[i].count = newCount;
                } else {
                    printf("So luong sach khong hop le, giu nguyen gia tri cu.\n");
                }
            }

            writeBooks(filename, n, book);
            printf("Da cap nhat thong tin sach.\n");
            return;
        }
    }
    printf("Khong tim thay ma sach de sua.\n");
}

void deleteBook(int idBook, int *n, Book *book, char filename[]) {
    int check = -1;
    for (int i = 0; i < *n; i++) {
        if (book[i].id == idBook) {
            check = i;
            break;
        }
    }
    if (check == -1) {
        printf("Khong tim thay sach can xoa\n");
        return;
    }
    for (int i = check; i < *n - 1; i++) {
        book[i] = book[i + 1];
    }
    (*n)--;
    writeBooks(filename, *n, book);
    printf("Xoa sach thanh cong\n");
}

void addBorrower(int *m, Borrower **borrower, char filename[]) {
    int newId;
    printf("\nNhap ma nguoi muon: ");
    scanf("%d", &newId);
    getchar();

    if (borrowerExists(newId, *m, *borrower)) {
        printf("Ma nguoi muon %d da ton tai. Khong the them moi.\n", newId);
        return;
    }

    *borrower = realloc(*borrower, sizeof(Borrower) * (*m + 1));
    Borrower *newBorrower = &(*borrower)[*m];

    newBorrower->id = newId;

    printf("\nNhap ten nguoi muon: ");
    fgets(newBorrower->name, 100, stdin);
    newBorrower->name[strcspn(newBorrower->name, "\n")] = 0;

    (*m)++;
    writeBorrowers(filename, *m, *borrower);
    printf("Da them nguoi muon moi thanh cong.\n");
}

void editBorrower(int idName, int m, Borrower *borrower, char filename[]) {
    for (int i = 0; i < m; i++) {
        if (borrower[i].id == idName) {
            printf("Nhap ten nguoi muon moi: ");
            fgets(borrower[i].name, sizeof(borrower[i].name), stdin);
            borrower[i].name[strcspn(borrower[i].name, "\n")] = '\0';
            writeBorrowers(filename, m, borrower);
            printf("Da cap nhat thong tin nguoi muon.\n");
            return;
        }
    }
    printf("Khong co ma nguoi muon tren.\n");
}

void deleteBorrower(int idName, int *m, Borrower *borrower, char filename[]) {
    int check = -1;
    for (int i = 0; i < *m; i++) {
        if (borrower[i].id == idName) {
            check = i;
            break;
        }
    }
    if (check == -1) {
        printf("Khong tim thay nguoi can xoa\n");
        return;
    }
    for (int i = check; i < *m - 1; i++) {
        borrower[i] = borrower[i + 1];
    }
    (*m)--;
    writeBorrowers(filename, *m, borrower);
    printf("Xoa nguoi muon thanh cong\n");
}

//muon tra sach
void writeBorrowBook(char filename[], Phieu phieu) {
    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    fprintf(f, "%d %d %d %d/%d/%d\n", phieu.id, phieu.idName, phieu.idBook, phieu.ngay, phieu.thang, phieu.nam);
    fclose(f);
}

void deleteBorrowRecord(char filename[], int idPhieu) {
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) {
        printf("Error opening files!");
        exit(1);
    }

    int id, idName, idBook, ngay, thang, nam;
    while (fscanf(f, "%d %d %d %d/%d/%d\n", &id, &idName, &idBook, &ngay, &thang, &nam) == 6) {
        if (id != idPhieu) {
            fprintf(temp, "%d %d %d %d/%d/%d\n", id, idName, idBook, ngay, thang, nam);
        }
    }
    fclose(f);
    fclose(temp);

    remove(filename);
    rename("temp.txt", filename);
}

int phieuExists(int idPhieu, char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error opening file %s!\n", filename);
        return 0;
    }

    int id;
    while (fscanf(f, "%d", &id) == 1) {
        if (id == idPhieu) {
            fclose(f);
            return 1;
        }
        char c;
        while ((c = fgetc(f)) != '\n' && c != EOF);
    }
    fclose(f);
    return 0;  
}

void borrowBook(int borrowerId, int bookId, Book *book, int n, Borrower *borrower, int m) {
    int borrowerFound = 0, bookFound = 0;
    char borrowerName[100];
    for (int i = 0; i < m; i++) {
        if (borrowerId == borrower[i].id) {
            borrowerFound = 1;
            strcpy(borrowerName, borrower[i].name);
            break;
        }
    }
    for (int i = 0; i < n; i++) {
        if (bookId == book[i].id) {
            bookFound = 1;
            if (book[i].count > 0) {
                book[i].count--;
                writeBooks("testBook.txt", n, book);
            } else {
                printf("Book is out of stock!\n");
                return;
            }
            break;
        }
    }
    if (!borrowerFound || !bookFound) {
        printf("Borrower or Book ID not found!\n");
        return;
    }
    Phieu phieu;
    while (1) {
        printf("Enter borrow record ID: ");
        scanf("%d", &phieu.id);
        
        if (phieuExists(phieu.id, "borrowBook.txt")) {
            printf("Borrow record ID %d already exists. Please choose another ID.\n", phieu.id);
        } else {
            break;
        }
    }
    phieu.idName = borrowerId;
    phieu.idBook = bookId;
    printf("Enter day, month, year of borrowing: ");
    scanf("%d%d%d", &phieu.ngay, &phieu.thang, &phieu.nam);
    
    writeBorrowBook("borrowBook.txt", phieu);
    
    printf("Book borrowed successfully by %s with record ID %d.\n", borrowerName, phieu.id);
}

void returnBook(int idPhieu, Book *book, int n, Phieu *phieu, int phieuCount) {
    FILE *f = fopen("borrowBook.txt", "r");
    if (f == NULL) {
        printf("Error opening borrowBook.txt!");
        exit(1);
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error opening temp.txt!");
        exit(1);
    }

    int check = 0;
    int tmpid;
    int id, idName, idBook, ngay, thang, nam;
    while (fscanf(f, "%d %d %d %d/%d/%d\n", &id, &idName, &idBook, &ngay, &thang, &nam) == 6) {
        if (id == idPhieu) {
            check = 1;
            tmpid = idBook;
        } else {
            fprintf(temp, "%d %d %d %d/%d/%d\n", id, idName, idBook, ngay, thang, nam);
        }
    }
    fclose(f);
    fclose(temp);
    
    remove("borrowBook.txt");
    rename("temp.txt", "borrowBook.txt");

    if (check == 0) {
        printf("khong co ma sach dang muon.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (book[i].id == tmpid) {
            book[i].count++;
            writeBooks("testBook.txt", n, book);
            break;
        }
    }

    printf("Book returned successfully.\n");
}

void displayBorrowedBooks(int idPhieu, Book *book, int n) {
    FILE *f = fopen("borrowBook.txt", "r");
    if (f == NULL) {
        printf("Error opening borrowBook.txt!");
        exit(1);
    }
    
    int id, idName, idBook, ngay, thang, nam;
    int found = 0;
    printf("\nBorrowed books for record ID %d:\n", idPhieu);
    while (fscanf(f, "%d %d %d %d/%d/%d\n", &id, &idName, &idBook, &ngay, &thang, &nam) == 6) {
        if (id == idPhieu) {
            found = 1;
            for (int i = 0; i < n; i++) {
                if (book[i].id == idBook) {
                    printf("Book ID: %d\nTitle: %s\nAuthor: %s\nBorrowed on: %d/%d/%d\n\n", 
                           book[i].id, book[i].title, book[i].author, ngay, thang, nam);
                    break;
                }
            }
        }
    }
    fclose(f);

    if (!found) {
        printf("No books found for this record ID.\n");
    }
}

void listAllBook(int n, Book *book) {
    printf("\nDanh sach sach trong thu vien: \n");
    printf("%-5s %-15s %-15s %-45s %s\n", "STT", "Ma", "So luong", "Ten", "Tac gia");
    for(int i = 0; i < n; i++) {
        printf("%-5d %-15d %-15d %-45s %s \n", i + 1, book[i].id, book[i].count, book[i].title, book[i].author);
    }
}

void listAllBorrower(int m, Borrower *borrower) {
    printf("\nDanh sach nguoi muon: \n");
    printf("%-5s %-15s %s\n", "STT", "Ma", "Ten");
    for (int i = 0; i < m; i++) {
        printf("%-5d %-15d %s\n", i + 1, borrower[i].id, borrower[i].name);
    }
}

void listBorrowedBySb(int idName, int m, Borrower *borrower, Book *book, int n) {
    FILE *f = fopen("borrowBook.txt", "r");
    if (f == NULL) {
        printf("Error opening borrowBook.txt!");
        exit(1);
    }
    
    int id, bId, bBook, ngay, thang, nam;
    int check = 0;
    printf("\nDanh sach sach dang muon cua nguoi co ma %d:\n", idName);
    while (fscanf(f, "%d %d %d %d/%d/%d\n", &id, &bId, &bBook, &ngay, &thang, &nam) == 6) {
        if (bId == idName) {
            check = 1;
            printf("ID phieu: %d\nNgay muon: %d/%d/%d\n\n", id, ngay, thang, nam);
        }
    }
    fclose(f);
    if (check == 0) {
        printf("Khong co sach nao dang muon\n");
    }
}



int main() {
    int n = 0, m = 0, phieuCount = 0;
    Book *book = NULL;
    Borrower *borrower = NULL;
    Phieu *phieu = NULL;
    inputBook("testBook.txt", &n, &book);
    inputBorrower("testname.txt", &m, &borrower);

    int choice, idBook, idName, idPhieu;
    while (1) {
        printf ("\n**************************************************\n");
        printf ("*   Plese select the number                      *\n");
        printf("*     [1]. Add book                              *\n");
        printf("*     [2]. Edit book                             *\n");
        printf("*     [3]. Delete book                           *\n");
        printf("*     [4]. Add borrower                          *\n");
        printf("*     [5]. Edit borrower                         *\n");
        printf("*     [6]. Delete borrower                       *\n");
        printf("*     [7]. Borrow book                           *\n");
        printf("*     [8]. Return book                           *\n");
        printf("*     [9]. Display all books                     *\n");
        printf("*    [10]. Display all borrowers                 *\n");
        printf("*    [11]. Display borrowed books by someone     *\n");
        printf("*    [12]. Exit                                  *\n");
        printf ("**************************************************\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addBook(&n, &book,"testBook.txt");
                break;
            case 2:
                printf("Nhap ma sach muon sua: ");
                scanf("%d", &idBook);
                getchar();
                editBook(idBook, n, book,"testBook.txt");
                break;
            case 3:
                printf("Nhap ma sach muon xoa: ");
                scanf("%d", &idBook);
                getchar();
                deleteBook(idBook, &n, book,"testBook.txt");
                break;
            case 4:
                addBorrower(&m, &borrower, "testname.txt");
                break;
            case 5:
                printf("Nhap ma nguoi muon muon sua: ");
                scanf("%d", &idName);
                getchar();
                editBorrower(idName, m, borrower, "testname.txt");
                break;
            case 6:
                printf("Nhap ma nguoi muon muon xoa: ");
                scanf("%d", &idName);
                getchar();
                deleteBorrower(idName, &m, borrower, "testname.txt");
                break;
            case 7:
                printf("Enter borrower ID: ");
                scanf("%d", &idName);
                printf("Enter book ID: ");
                scanf("%d", &idBook);
                getchar();
                borrowBook(idName, idBook, book, n, borrower, m);
                break;
            case 8:
                printf("Enter the ID phieu to return: ");
                scanf("%d", &idPhieu);
                getchar();
                returnBook(idPhieu, book, n, phieu, phieuCount);
                break;
            case 9:
                listAllBook(n, book);
                break;
            case 10:
                listAllBorrower(m, borrower);
                break;
            case 11:
                printf("Nhap ma nguoi muon ban muon xem so sach: ");
                scanf("%d", &idName);
                getchar();
                listBorrowedBySb(idName, m, borrower, book, n);
                break;
            case 12:
                free(book);
                free(borrower);
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
