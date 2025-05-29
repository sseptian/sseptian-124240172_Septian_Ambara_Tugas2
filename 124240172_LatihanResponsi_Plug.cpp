#include <iostream>
#include <stack>
#include <string>
using namespace std;

struct Node {
    string nama, posisi;
    int nominal;
    Node *left = nullptr, *right = nullptr;
    Node(string n, string p, int d) : nama(n), posisi(p), nominal(d) {}
};

struct Aksi {
    string jenis;
    Node* data;
};

Node* root = nullptr;
stack<Aksi> logUndo;

Node* insert(Node* node, string nama, string posisi, int nominal) {
    if (!node) return new Node(nama, posisi, nominal);
    if (nama < node->nama)
        node->left = insert(node->left, nama, posisi, nominal);
    else
        node->right = insert(node->right, nama, posisi, nominal);
    return node;
}

void inorder(Node* node) {
    if (!node) return;
    inorder(node->left);
    cout << "Nama: " << node->nama << ", Posisi: " << node->posisi << ", Dana: " << node->nominal << endl;
    inorder(node->right);
}

Node* findMin(Node* node) {
    while (node && node->left) node = node->left;
    return node;
}

Node* remove(Node* node, string nama, Node*& deletedNode) {
    if (!node) return nullptr;
    if (nama < node->nama)
        node->left = remove(node->left, nama, deletedNode);
    else if (nama > node->nama)
        node->right = remove(node->right, nama, deletedNode);
    else {
        deletedNode = new Node(node->nama, node->posisi, node->nominal);
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }
        Node* temp = findMin(node->right);
        node->nama = temp->nama;
        node->posisi = temp->posisi;
        node->nominal = temp->nominal;
        node->right = remove(node->right, temp->nama, deletedNode);
    }
    return node;
}

void undo() {
    if (logUndo.empty()) {
        cout << "Tidak ada aksi untuk di-undo.\n";
        return;
    }
    Aksi aksi = logUndo.top(); logUndo.pop();
    if (aksi.jenis == "tambah") {
        Node* dummy = nullptr;
        root = remove(root, aksi.data->nama, dummy);
        cout << "Undo penambahan berhasil.\n";
    } else if (aksi.jenis == "hapus") {
        root = insert(root, aksi.data->nama, aksi.data->posisi, aksi.data->nominal);
        cout << "Undo penghapusan berhasil.\n";
    }
}

int main() {
    int pilihan;
    do {
        cout << "\nMenu:\n1. Tambah Data\n2. Tampilkan Data\n3. Hapus Data\n4. Undo\n0. Keluar\nPilihan: ";
        cin >> pilihan; cin.ignore();
        if (pilihan == 1) {
            string nama, posisi; int nominal;
            cout << "Nama: "; getline(cin, nama);
            cout << "Posisi: "; getline(cin, posisi);
            cout << "Nominal: "; cin >> nominal; cin.ignore();
            root = insert(root, nama, posisi, nominal);
            logUndo.push({"tambah", new Node(nama, posisi, nominal)});
        } else if (pilihan == 2) {
            cout << "Daftar Terduga Pelaku (Inorder A-Z):\n";
            inorder(root);
        } else if (pilihan == 3) {
            string nama;
            cout << "Nama yang akan dihapus: "; getline(cin, nama);
            Node* deletedNode = nullptr;
            root = remove(root, nama, deletedNode);
            if (deletedNode) {
                logUndo.push({"hapus", deletedNode});
                cout << "Data berhasil dihapus.\n";
            } else {
                cout << "Data tidak ditemukan.\n";
            }
        } else if (pilihan == 4) {
            undo();
        }
    } while (pilihan != 0);
    cout << "Program selesai\n";
    return 0;
}
