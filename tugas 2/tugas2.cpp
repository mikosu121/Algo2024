#include <iostream>
#include <iomanip>
#include <string>
#include <fstream> 
using namespace std;

struct Barang {
    string nama;
    string kode;
    int harga;
    int jumlah;
    Barang* next;
};

Barang* head = nullptr;

// === Fungsi untuk menyimpan data ke file ===
void simpanKeFile() {
    ofstream file("stok.txt");
    if (!file) {
        cerr << "Gagal membuka file untuk menyimpan data.\n";
        return;
    }

    Barang* temp = head;
    while (temp) {
        file << temp->nama << "|"
             << temp->kode << "|"
             << temp->harga << "|"
             << temp->jumlah << "\n";
        temp = temp->next;
    }

    file.close();
}

// === Fungsi untuk memuat data dari file saat program dijalankan ===
void muatDariFile() {
    ifstream file("stok.txt");
    if (!file) return;

    string line;
    while (getline(file, line)) {
        Barang* baru = new Barang();
        size_t pos = 0;

        pos = line.find('|'); baru->nama = line.substr(0, pos); line.erase(0, pos + 1);
        pos = line.find('|'); baru->kode = line.substr(0, pos); line.erase(0, pos + 1);
        pos = line.find('|'); baru->harga = stoi(line.substr(0, pos)); line.erase(0, pos + 1);
        baru->jumlah = stoi(line);
        baru->next = nullptr;

        if (!head) {
            head = baru;
        } else {
            Barang* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }

    file.close();
}

void tambahBarang() {
    int jumlahBarang;
    cout << "Berapa jumlah barang yang ingin ditambahkan? ";
    cin >> jumlahBarang;

    for (int i = 0; i < jumlahBarang; i++) {
        Barang* baru = new Barang();
        cout << "\n=== Input Barang ke-" << (i + 1) << " ===\n";
        cin.ignore();
        cout << "Nama Barang : "; getline(cin, baru->nama);
        cout << "Kode Barang : "; getline(cin, baru->kode);
        cout << "Harga       : "; cin >> baru->harga;
        cout << "Jumlah Stok : "; cin >> baru->jumlah;
        baru->next = nullptr;

        if (!head) {
            head = baru;
        } else {
            Barang* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }

    cout << "\nBarang berhasil ditambahkan!\n";
    simpanKeFile();  // Simpan otomatis
}

void tampilBarang(bool termahal = false) {
    if (!head) {
        cout << "Stok kosong.\n";
        return;
    }

    int count = 0;
    Barang* temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }

    Barang** arr = new Barang*[count];
    temp = head;
    for (int i = 0; i < count; ++i) {
        arr[i] = temp;
        temp = temp->next;
    }

    // Bubble Sort berdasarkan harga
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if ((termahal && arr[j]->harga < arr[j + 1]->harga) ||
                (!termahal && arr[j]->harga > arr[j + 1]->harga)) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }

    cout << left << setw(20) << "Nama"
         << setw(15) << "Kode"
         << setw(10) << "Harga"
         << setw(10) << "Jumlah" << endl;
    cout << string(55, '-') << endl;

    for (int i = 0; i < count; ++i) {
        cout << left << setw(20) << arr[i]->nama
             << setw(15) << arr[i]->kode
             << setw(10) << arr[i]->harga
             << setw(10) << arr[i]->jumlah << endl;
    }

    delete[] arr;
}

void hapusBarang() {
    if (!head) {
        cout << "Stok kosong.\n";
        return;
    }

    string namaHapus;
    cout << "Masukkan nama barang yang ingin dihapus: ";
    cin.ignore(); getline(cin, namaHapus);

    Barang* temp = head;
    Barang* prev = nullptr;

    while (temp && temp->nama != namaHapus) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Barang dengan nama \"" << namaHapus << "\" tidak ditemukan.\n";
        return;
    }

    if (!prev) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;
    cout << "Barang berhasil dihapus.\n";
    simpanKeFile(); // Simpan otomatis setelah hapus
    cout << "\nStok Barang Saat Ini:\n";
    tampilBarang();
}

void menu() {
    int pilih;
    do {
        cout << "\n====== MENU STOK BARANG ======\n";
        cout << "1. Input Data Barang\n";
        cout << "2. Lihat Data Barang\n";
        cout << "3. Hapus Data Barang\n";
        cout << "4. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch (pilih) {
            case 1: tambahBarang(); break;
            case 2: {
                int sort;
                cout << "1. Urutkan dari Termurah\n2. Urutkan dari Termahal\nPilih: ";
                cin >> sort;
                tampilBarang(sort == 2);
                break;
            }
            case 3: hapusBarang(); break;
            case 4: cout << "Keluar dari program.\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilih != 4);
}

int main() {
    muatDariFile(); // Muat data saat program dimulai
    menu();
    return 0;
}
