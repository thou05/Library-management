#include <gtk/gtk.h>
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

typedef struct {
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

void writeBorrowBook(char filename[], Phieu phieu) {
    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        printf("Error opening file!");
        exit(1);
    }
    fprintf(f, "%d %d %d %d/%d/%d\n", phieu.id, phieu.idName, phieu.idBook, phieu.ngay, phieu.thang, phieu.nam);
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

void addBook(int *n, Book **book, char filename[], GtkWidget *entry_id, GtkWidget *entry_title, GtkWidget *entry_author, GtkWidget *entry_count) {
    int newId = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
    const char *newTitle = gtk_entry_get_text(GTK_ENTRY(entry_title));
    const char *newAuthor = gtk_entry_get_text(GTK_ENTRY(entry_author));
    int newCount = atoi(gtk_entry_get_text(GTK_ENTRY(entry_count)));

    if (bookExists(newId, *n, *book)) {
        printf("Ma sach %d da ton tai. Khong the them moi.\n", newId);
        return;
    }

    *book = realloc(*book, sizeof(Book) * (*n + 1));
    Book *newBook = &(*book)[*n];
    newBook->id = newId;
    strcpy(newBook->title, newTitle);
    strcpy(newBook->author, newAuthor);
    newBook->count = newCount;

    (*n)++;
    writeBooks(filename, *n, *book);
    printf("Da them sach moi thanh cong.\n");
}

void editBook(int idBook, int n, Book *book, char filename[], GtkWidget *entry_title, GtkWidget *entry_author, GtkWidget *entry_count) {
    for (int i = 0; i < n; i++) {
        if (book[i].id == idBook) {
            const char *newTitle = gtk_entry_get_text(GTK_ENTRY(entry_title));
            const char *newAuthor = gtk_entry_get_text(GTK_ENTRY(entry_author));
            int newCount = atoi(gtk_entry_get_text(GTK_ENTRY(entry_count)));

            if (strlen(newTitle) > 0) {
                strcpy(book[i].title, newTitle);
            }
            if (strlen(newAuthor) > 0) {
                strcpy(book[i].author, newAuthor);
            }
            if (newCount >= 0) {
                book[i].count = newCount;
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

void addBorrower(int *m, Borrower **borrower, char filename[], GtkWidget *entry_id, GtkWidget *entry_name) {
    int newId = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
    const char *newName = gtk_entry_get_text(GTK_ENTRY(entry_name));

    if (borrowerExists(newId, *m, *borrower)) {
        printf("Ma nguoi muon %d da ton tai. Khong the them moi.\n", newId);
        return;
    }

    *borrower = realloc(*borrower, sizeof(Borrower) * (*m + 1));
    Borrower *newBorrower = &(*borrower)[*m];

    newBorrower->id = newId;
    strcpy(newBorrower->name, newName);

    (*m)++;
    writeBorrowers(filename, *m, *borrower);
    printf("Da them nguoi muon moi thanh cong.\n");
}

void editBorrower(int idName, int m, Borrower *borrower, char filename[], GtkWidget *entry_name) {
    for (int i = 0; i < m; i++) {
        if (borrower[i].id == idName) {
            const char *newName = gtk_entry_get_text(GTK_ENTRY(entry_name));
            strcpy(borrower[i].name, newName);
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

void borrowBook(int borrowerId, int bookId, Book *book, int n, Borrower *borrower, int m, GtkWidget *entry_id, GtkWidget *entry_day, GtkWidget *entry_month, GtkWidget *entry_year) {
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
    phieu.id = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
    if (phieuExists(phieu.id, "borrowBook.txt")) {
        printf("Borrow record ID %d already exists. Please choose another ID.\n", phieu.id);
        return;
    }
    phieu.idName = borrowerId;
    phieu.idBook = bookId;
    phieu.ngay = atoi(gtk_entry_get_text(GTK_ENTRY(entry_day)));
    phieu.thang = atoi(gtk_entry_get_text(GTK_ENTRY(entry_month)));
    phieu.nam = atoi(gtk_entry_get_text(GTK_ENTRY(entry_year)));

    writeBorrowBook("borrowBook.txt", phieu);

    printf("Book borrowed successfully by %s with record ID %d.\n", borrowerName, phieu.id);
}

void returnBook(int idPhieu, Book *book, int n) {
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

void listAllBook(int n, Book *book) {
    printf("\nDanh sach sach trong thu vien: \n");
    printf("%-5s %-15s %-15s %-45s %s\n", "STT", "Ma", "So luong", "Ten", "Tac gia");
    for (int i = 0; i < n; i++) {
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

void show_add_book_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id, *entry_title, *entry_author, *entry_count;

    dialog = gtk_dialog_new_with_buttons("Add Book",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Book ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Title:"), 0, 1, 1, 1);
    entry_title = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_title, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Author:"), 0, 2, 1, 1);
    entry_author = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_author, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Count:"), 0, 3, 1, 1);
    entry_count = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_count, 1, 3, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int *n = (int *)g_object_get_data(G_OBJECT(data), "n");
        Book **book = (Book **)g_object_get_data(G_OBJECT(data), "book");
        addBook(n, book, "testBook.txt", entry_id, entry_title, entry_author, entry_count);
    }

    gtk_widget_destroy(dialog);
}

void show_edit_book_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id, *entry_title, *entry_author, *entry_count;

    dialog = gtk_dialog_new_with_buttons("Edit Book",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Book ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("New Title:"), 0, 1, 1, 1);
    entry_title = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_title, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("New Author:"), 0, 2, 1, 1);
    entry_author = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_author, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("New Count:"), 0, 3, 1, 1);
    entry_count = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_count, 1, 3, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int idBook = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        int *n = (int *)g_object_get_data(G_OBJECT(data), "n");
        Book *book = (Book *)g_object_get_data(G_OBJECT(data), "book");
        editBook(idBook, *n, book, "testBook.txt", entry_title, entry_author, entry_count);
    }

    gtk_widget_destroy(dialog);
}

void show_delete_book_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id;

    dialog = gtk_dialog_new_with_buttons("Delete Book",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Book ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int idBook = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        int *n = (int *)g_object_get_data(G_OBJECT(data), "n");
        Book *book = (Book *)g_object_get_data(G_OBJECT(data), "book");
        deleteBook(idBook, n, book, "testBook.txt");
    }

    gtk_widget_destroy(dialog);
}

void show_add_borrower_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id, *entry_name;

    dialog = gtk_dialog_new_with_buttons("Add Borrower",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Borrower ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Name:"), 0, 1, 1, 1);
    entry_name = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int *m = (int *)g_object_get_data(G_OBJECT(data), "m");
        Borrower **borrower = (Borrower **)g_object_get_data(G_OBJECT(data), "borrower");
        addBorrower(m, borrower, "testname.txt", entry_id, entry_name);
    }

    gtk_widget_destroy(dialog);
}

void show_edit_borrower_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id, *entry_name;

    dialog = gtk_dialog_new_with_buttons("Edit Borrower",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Borrower ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("New Name:"), 0, 1, 1, 1);
    entry_name = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_name, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int idName = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        int *m = (int *)g_object_get_data(G_OBJECT(data), "m");
        Borrower *borrower = (Borrower *)g_object_get_data(G_OBJECT(data), "borrower");
        editBorrower(idName, *m, borrower, "testname.txt", entry_name);
    }

    gtk_widget_destroy(dialog);
}

void show_delete_borrower_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id;

    dialog = gtk_dialog_new_with_buttons("Delete Borrower",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Borrower ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int idName = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        int *m = (int *)g_object_get_data(G_OBJECT(data), "m");
        Borrower *borrower = (Borrower *)g_object_get_data(G_OBJECT(data), "borrower");
        deleteBorrower(idName, m, borrower, "testname.txt");
    }

    gtk_widget_destroy(dialog);
}

void show_borrow_book_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id, *entry_borrower_id, *entry_book_id, *entry_day, *entry_month, *entry_year;

    dialog = gtk_dialog_new_with_buttons("Borrow Book",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Record ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Borrower ID:"), 0, 1, 1, 1);
    entry_borrower_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_borrower_id, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Book ID:"), 0, 2, 1, 1);
    entry_book_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_book_id, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Day:"), 0, 3, 1, 1);
    entry_day = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_day, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Month:"), 0, 4, 1, 1);
    entry_month = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_month, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Year:"), 0, 5, 1, 1);
    entry_year = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_year, 1, 5, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int borrowerId = atoi(gtk_entry_get_text(GTK_ENTRY(entry_borrower_id)));
        int bookId = atoi(gtk_entry_get_text(GTK_ENTRY(entry_book_id)));
        int *n = (int *)g_object_get_data(G_OBJECT(data), "n");
        int *m = (int *)g_object_get_data(G_OBJECT(data), "m");
        Book *book = (Book *)g_object_get_data(G_OBJECT(data), "book");
        Borrower *borrower = (Borrower *)g_object_get_data(G_OBJECT(data), "borrower");
        borrowBook(borrowerId, bookId, book, *n, borrower, *m, entry_id, entry_day, entry_month, entry_year);
    }

    gtk_widget_destroy(dialog);
}

void show_return_book_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog, *content_area, *grid;
    GtkWidget *entry_id;

    dialog = gtk_dialog_new_with_buttons("Return Book",
                                         GTK_WINDOW(data),
                                         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                         ("OK"),
                                         GTK_RESPONSE_OK,
                                         ("Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    grid = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Record ID:"), 0, 0, 1, 1);
    entry_id = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_id, 1, 0, 1, 1);

    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        int idPhieu = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id)));
        int *n = (int *)g_object_get_data(G_OBJECT(data), "n");
        Book *book = (Book *)g_object_get_data(G_OBJECT(data), "book");
        returnBook(idPhieu, book, *n);
    }

    gtk_widget_destroy(dialog);
}


// // Function to show the list of all books
// void show_book_list(GtkWidget *widget, gpointer data) {
//     int n = *(int *)data;
//     Book *book = (Book *)g_object_get_data(G_OBJECT(widget), "book");
//     listAllBook(n, book);
// }

// // Function to show the list of all borrowers
// void show_borrower_list(GtkWidget *widget, gpointer data) {
//     int m = *(int *)data;
//     Borrower *borrower = (Borrower *)g_object_get_data(G_OBJECT(widget), "borrower");
//     listAllBorrower(m, borrower);
// }


// Callback function for the "Quit" menu item
void on_quit(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    int n = 0, m = 0;
    Book *book = NULL;
    Borrower *borrower = NULL;

    inputBook("testBook.txt", &n, &book);
    inputBorrower("testname.txt", &m, &borrower);

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menu_bar;
    GtkWidget *menu;
    GtkWidget *menu_item;
    GtkWidget *menu_item_add_book;
    GtkWidget *menu_item_edit_book;
    GtkWidget *menu_item_delete_book;
    GtkWidget *menu_item_add_borrower;
    GtkWidget *menu_item_edit_borrower;
    GtkWidget *menu_item_delete_borrower;
    GtkWidget *menu_item_borrow_book;
    GtkWidget *menu_item_return_book;
    GtkWidget *menu_item_list_books;
    GtkWidget *menu_item_list_borrowers;
    GtkWidget *menu_item_quit;
    GtkWidget *menu2;
    GtkWidget *menu2_item;
    GtkWidget *menu3;
    GtkWidget *menu3_item;
    GtkWidget *menu4;
    GtkWidget *menu4_item;



    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Library Management");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    // Connect the window's destroy signal to the gtk_main_quit function
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a menu bar
    menu_bar = gtk_menu_bar_new();

    // Create the main menu
    menu = gtk_menu_new();

    // Create the "Library" menu item and attach the submenu to it
    menu_item = gtk_menu_item_new_with_label("Book");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);

    
    // Create the submenu for "Manage" menu item
    menu2 = gtk_menu_new();
    menu2_item = gtk_menu_item_new_with_label("Borrower");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu2_item), menu2);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu2_item);

    // Create the submenu for "Manage" menu item
    menu3 = gtk_menu_new();
    menu3_item = gtk_menu_item_new_with_label("Manage");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu3_item), menu3);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu3_item);

    // // Create the submenu for "List" menu item
    // menu4 = gtk_menu_new();
    // menu4_item = gtk_menu_item_new_with_label("List");
    // gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu4_item), menu4);
    // gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu4_item);

    // Create and add the "Add Book" menu item
    menu_item_add_book = gtk_menu_item_new_with_label("Add Book");
    g_signal_connect(menu_item_add_book, "activate", G_CALLBACK(show_add_book_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_add_book);

    // Create and add the "Edit Book" menu item
    menu_item_edit_book = gtk_menu_item_new_with_label("Edit Book");
    g_signal_connect(menu_item_edit_book, "activate", G_CALLBACK(show_edit_book_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_edit_book);

    // Create and add the "Delete Book" menu item
    menu_item_delete_book = gtk_menu_item_new_with_label("Delete Book");
    g_signal_connect(menu_item_delete_book, "activate", G_CALLBACK(show_delete_book_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item_delete_book);

    // Create and add the "Add Borrower" menu item
    menu_item_add_borrower = gtk_menu_item_new_with_label("Add Borrower");
    g_signal_connect(menu_item_add_borrower, "activate", G_CALLBACK(show_add_borrower_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), menu_item_add_borrower);

    // Create and add the "Edit Borrower" menu item
    menu_item_edit_borrower = gtk_menu_item_new_with_label("Edit Borrower");
    g_signal_connect(menu_item_edit_borrower, "activate", G_CALLBACK(show_edit_borrower_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), menu_item_edit_borrower);

    // Create and add the "Delete Borrower" menu item
    menu_item_delete_borrower = gtk_menu_item_new_with_label("Delete Borrower");
    g_signal_connect(menu_item_delete_borrower, "activate", G_CALLBACK(show_delete_borrower_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu2), menu_item_delete_borrower);

    // Create and add the "Borrow Book" menu item
    menu_item_borrow_book = gtk_menu_item_new_with_label("Borrow Book");
    g_signal_connect(menu_item_borrow_book, "activate", G_CALLBACK(show_borrow_book_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), menu_item_borrow_book);

    // Create and add the "Return Book" menu item
    menu_item_return_book = gtk_menu_item_new_with_label("Return Book");
    g_signal_connect(menu_item_return_book, "activate", G_CALLBACK(show_return_book_dialog), window);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu3), menu_item_return_book);

    // // Create and add the "List All Books" menu item directly to the menu bar
    // menu_item_list_books = gtk_menu_item_new_with_label("List All Books");
    // g_signal_connect(menu_item_list_books, "activate", G_CALLBACK(show_book_list), &n);
    // gtk_menu_shell_append(GTK_MENU_SHELL(menu4), menu_item_list_books);

    // // Create and add the "List All Borrowers" menu item directly to the menu bar
    // menu_item_list_borrowers = gtk_menu_item_new_with_label("List All Borrowers");
    // g_signal_connect(menu_item_list_borrowers, "activate", G_CALLBACK(show_borrower_list), &m);
    // gtk_menu_shell_append(GTK_MENU_SHELL(menu4), menu_item_list_borrowers);


    // Create and add the "Quit" menu item
    menu_item_quit = gtk_menu_item_new_with_label("Quit");
    g_signal_connect(menu_item_quit, "activate", G_CALLBACK(on_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item_quit);

    // Add the "Library" menu item to the menu bar
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu2_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu3_item);
    //gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu4_item);

    

    // Pack the menu bar into the vertical box container
    gtk_box_pack_start(GTK_BOX(vbox), menu_bar, FALSE, FALSE, 0);

    // Set custom data to the window for passing to callback functions
    g_object_set_data(G_OBJECT(window), "n", &n);
    g_object_set_data(G_OBJECT(window), "book", book);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the main GTK loop
    gtk_main();

    // Free dynamically allocated memory
    free(book);
    free(borrower);

    return 0;
}