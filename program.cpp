#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <iomanip> // Untuk merapikan tabel

using namespace std;

// 1. STRUCT UNTUK DATA LENGKAP
struct Item {
    string id;
    string nama;
    double harga;
    int stok;
};

// --- VARIABEL GLOBAL (DATABASE SEDERHANA) ---
const int MAX_ITEMS = 100; // Batas maksimum barang
Item gudang[MAX_ITEMS];    // 2. ARRAY 1D UNTUK DATA BARANG
int jumlahBarang = 0;      // Counter jumlah barang saat ini

stack<string> riwayatTransaksi; // 3. STACK UNTUK RIWAYAT (LIFO)
queue<string> antrianPelanggan; // 4. QUEUE UNTUK ANTRIAN (FIFO)

// --- FUNGSI-FUNGSI ---

// Fungsi Tambah Barang (Array + Input)
void tambahBarang() {
    if (jumlahBarang >= MAX_ITEMS) {
        cout << "[!] Gudang penuh!\n";
        return;
    }

    Item barangBaru;
    cout << "\n--- Tambah Barang Baru ---\n";
    cout << "ID Barang   : "; cin >> barangBaru.id;
    cin.ignore(); // Membersihkan buffer
    cout << "Nama Barang : "; getline(cin, barangBaru.nama);
    cout << "Harga       : Rp "; cin >> barangBaru.harga;
    cout << "Stok Awal   : "; cin >> barangBaru.stok;

    gudang[jumlahBarang] = barangBaru;
    jumlahBarang++;
    cout << "[v] Barang berhasil ditambahkan!\n";
}

// Fungsi Lihat Semua Barang (Perulangan + Array)
void lihatBarang() {
    cout << "\n--- Daftar Barang TokoKu ---\n";
    cout << left << setw(10) << "ID" 
         << left << setw(20) << "Nama" 
         << left << setw(15) << "Harga" 
         << left << setw(10) << "Stok" << endl;
    cout << "------------------------------------------------------\n";

    if (jumlahBarang == 0) {
        cout << " (Gudang Kosong) \n";
    } else {
        for (int i = 0; i < jumlahBarang; i++) {
            cout << left << setw(10) << gudang[i].id 
                 << left << setw(20) << gudang[i].nama 
                 << left << setw(15) << gudang[i].harga 
                 << left << setw(10) << gudang[i].stok << endl;
        }
    }
}

// Fungsi Cari Barang (Percabangan + Perulangan)
// Mengembalikan index barang di array, atau -1 jika tidak ketemu
int cariIndexBarang(string idCari) {
    for (int i = 0; i < jumlahBarang; i++) {
        if (gudang[i].id == idCari) {
            return i;
        }
    }
    return -1;
}

void cariBarangUI() {
    string id;
    cout << "\nMasukkan ID Barang yang dicari: "; cin >> id;
    int idx = cariIndexBarang(id);
    
    if (idx != -1) {
        cout << "[v] Ditemukan: " << gudang[idx].nama 
             << " | Stok: " << gudang[idx].stok 
             << " | Harga: " << gudang[idx].harga << endl;
    } else {
        cout << "[!] Barang tidak ditemukan.\n";
    }
}

// Fungsi Tambah Antrian (Queue Push)
void ambilAntrian() {
    string namaPelanggan;
    cout << "\nMasukkan Nama Pelanggan: ";
    // cin.ignore(); // Sudah di-ignore di main, jadi bisa dihapus di sini
    getline(cin, namaPelanggan);
    antrianPelanggan.push(namaPelanggan);
    cout << "[v] Pelanggan " << namaPelanggan << " masuk antrian.\n";
}

// Fungsi Proses Transaksi (Queue Pop + Stack Push)
void prosesAntrian() {
    if (antrianPelanggan.empty()) {
        cout << "[!] Tidak ada antrian pelanggan.\n";
        return;
    }

    string pelanggan = antrianPelanggan.front(); // Lihat orang paling depan
    cout << "\n--- Melayani Pelanggan: " << pelanggan << " ---\n";
    
    lihatBarang(); // Tampilkan menu agar mudah memilih

    string idBarang;
    int qty;
    cout << "Masukkan ID Barang yang dibeli: "; cin >> idBarang;
    
    int idx = cariIndexBarang(idBarang);

    if (idx != -1) {
        cout << "Masukkan Jumlah Beli: "; cin >> qty;
        
        if (gudang[idx].stok >= qty) {
            // Kurangi stok
            gudang[idx].stok -= qty;
            
            // Hitung total
            double total = gudang[idx].harga * qty;
            cout << "[v] Transaksi Berhasil! Total: Rp " << total << endl;

            // Simpan ke Riwayat (STACK)
            string log = "Pelanggan: " + pelanggan + " | Beli: " + gudang[idx].nama + " (" + to_string(qty) + " pcs)";
            riwayatTransaksi.push(log);

            // Keluarkan dari antrian (QUEUE)
            antrianPelanggan.pop(); 

        } else {
            cout << "[!] Stok tidak mencukupi!\n";
        }
    } else {
        cout << "[!] Barang tidak valid. Pelanggan masih dalam antrian.\n";
    }
}

// Fungsi Lihat Riwayat (Operasi Stack)
void lihatRiwayat() {
    cout << "\n--- Riwayat Transaksi Terbaru (LIFO) ---\n";
    if (riwayatTransaksi.empty()) {
        cout << "(Belum ada transaksi)\n";
        return;
    }

    // Kita copy stack agar stack asli tidak rusak saat ditampilkan
    stack<string> tempStack = riwayatTransaksi;
    
    while (!tempStack.empty()) {
        cout << ">> " << tempStack.top() << endl; // Lihat yang paling atas
        tempStack.pop(); // Buang sementara dari copy-an
    }
}

// Fungsi Cek Stok Menipis (Percabangan)
void cekStokMenipis() {
    cout << "\n--- Peringatan Stok Menipis (< 5) ---\n";
    bool ada = false;
    for (int i = 0; i < jumlahBarang; i++) {
        if (gudang[i].stok < 5) {
            cout << "[!] WARN: " << gudang[i].nama << " (Sisa: " << gudang[i].stok << ")\n";
            ada = true;
        }
    }
    if (!ada) cout << "[v] Semua stok aman.\n";
}

int main() {
    int pilihan;
    
    // Data Dummy untuk testing awal
    gudang[jumlahBarang++] = {"B01", "Beras 5kg", 65000, 10};
    gudang[jumlahBarang++] = {"G01", "Gula Pasir", 12500, 3}; // Stok tipis
    gudang[jumlahBarang++] = {"M01", "Minyak 2L", 35000, 20};

    do {
        cout << "\n===================================";
        cout << "\n   TOKOKU INVENTORY MANAGER";
        cout << "\n===================================";
        cout << "\n1. Lihat Semua Barang";
        cout << "\n2. Tambah Barang Baru";
        cout << "\n3. Cari Barang";
        cout << "\n4. Ambil Antrian (Pelanggan Datang)";
        cout << "\n5. Proses Antrian (Beli Barang)";
        cout << "\n6. Lihat Riwayat Transaksi";
        cout << "\n7. Cek Stok Menipis";
        cout << "\n0. Keluar";
        cout << "\n===================================";
        cout << "\nPilihan Anda: "; 
        cin >> pilihan;
        cin.ignore(); // Tambahkan ini untuk membersihkan buffer

        switch (pilihan) {
            case 1: lihatBarang(); break;
            case 2: tambahBarang(); break;
            case 3: cariBarangUI(); break;
            case 4: ambilAntrian(); break;
            case 5: prosesAntrian(); break;
            case 6: lihatRiwayat(); break;
            case 7: cekStokMenipis(); break;
            case 0: cout << "Terima kasih telah menggunakan TokoKu!\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}