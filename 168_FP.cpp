#include <iostream>
#include <string>
#include <fstream> 
using namespace std;

// 1. LINKED LIST: Inventaris Suku Cadang Kapal
// Struct untuk Node Linked List
struct PartNode {
    string partName;
    int quantity;
    PartNode* next;
};

// Struct untuk Sistem Inventaris (Mengelola Linked List)
struct InventorySystem {
    PartNode* head = nullptr; // Pointer awal list
    string filename = "inventaris_stasiun.txt";

   
    // 1. CREATE: Menambah suku cadang baru di akhir list
    void addPart(string name, int qty) {
        PartNode* newNode = new PartNode();
        newNode->partName = name;
        newNode->quantity = qty;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode; 
        } else {
            PartNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next; 
            }
            temp->next = newNode; 
        }
    }

    // 2. READ: Menampilkan semua isi inventaris
    void displayInventory() {
        if (head == nullptr) {
            cout << "Inventaris kosong." << endl;
            return;
        }
        cout << "\n=== DAFTAR INVENTARIS SUKU CADANG ===" << endl;
        PartNode* temp = head;
        int no = 1;
        while (temp != nullptr) {
            cout << no << ". " << temp->partName << " - Jumlah: " << temp->quantity << endl;
            temp = temp->next;
            no++;
        }
        cout << "=====================================\n" << endl;
    }


    // 3. UPDATE: Mengubah jumlah suku cadang berdasarkan nama
    void updatePart(string name, int newQty) {
        PartNode* temp = head;
        while (temp != nullptr) {
            if (temp->partName == name) {
                temp->quantity = newQty;
                cout << "Data " << name << " berhasil diubah menjadi " << newQty << "." << endl;
                saveToFile(); // Simpan perubahan ke file
                return;
            }
            temp = temp->next;
        }
        cout << "Suku cadang " << name << " tidak ditemukan!" << endl;
    }

    // 4. DELETE: Menghapus suku cadang dari list
    void deletePart(string name) {
        if (head == nullptr) return;

        // Jika yang dihapus adalah node pertama (head)
        if (head->partName == name) {
            PartNode* temp = head;
            head = head->next;
            delete temp; 
            cout << "Data " << name << " berhasil dihapus." << endl;
            saveToFile();
            return;
        }

        // Jika yang dihapus ada di tengah atau akhir
        PartNode* current = head;
        PartNode* prev = nullptr;
        while (current != nullptr && current->partName != name) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Suku cadang " << name << " tidak ditemukan!" << endl;
            return;
        }

        prev->next = current->next; 
        delete current;             
        cout << "Data " << name << " berhasil dihapus." << endl;
        saveToFile(); // Simpan perubahan ke file
    }

    // FITUR FILE I/O: Menyimpan data ke TXT
    void saveToFile() {
        ofstream file(filename); // Membuka file untuk ditulis (overwrite)
        if (file.is_open()) {
            PartNode* temp = head;
            while (temp != nullptr) {
                // Menyimpan dengan format: Nama_Barang Jumlah
                file << temp->partName << " " << temp->quantity << endl;
                temp = temp->next;
            }
            file.close();
        }
    }


    // FITUR FILE I/O: Memuat data dari TXT
    void loadFromFile() {
        ifstream file(filename); // Membuka file untuk dibaca
        string name;
        int qty;
        
        // Membaca file kata demi kata. 
        // Nama barang tidak memakai spasi (gunakan underscore, misal: Filter_Oksigen)
        while (file >> name >> qty) {
            addPart(name, qty);
        }
        file.close();
    }
};


// 2. QUEUE: Antrean Kapal Suplai Logistik (Prinsip FIFO: First In First Out)
// Struct untuk Node Antrean Kapal
struct SupplyShip {
    string shipName;
    string cargoType;
    SupplyShip* next;
};

// Struct untuk Sistem Antrean (Mengelola Queue)
struct SupplyQueueSystem {
    SupplyShip* front = nullptr; // Pointer kapal antrean terdepan
    SupplyShip* rear = nullptr;  // Pointer kapal antrean paling belakang
    string filename = "antrean_suplai.txt";


    // 1. ENQUEUE: Menambah kapal baru di belakang antrean
    void enqueue(string name, string cargo) {
        SupplyShip* newShip = new SupplyShip();
        newShip->shipName = name;
        newShip->cargoType = cargo;
        newShip->next = nullptr;

        // Jika antrean masih kosong
        if (rear == nullptr) {
            front = rear = newShip;
        } else {
            // Tambahkan di belakang rear yang sekarang
            rear->next = newShip; 
            rear = newShip;       
        }
    }

    // Fungsi tambahan (Wrapper) agar saat enqueue manual langsung tersimpan
    void addShip(string name, string cargo) {
        enqueue(name, cargo);
        cout << "Kapal " << name << " dengan muatan " << cargo << " masuk ke antrean." << endl;
        saveToFile();
    }

    // 2. DEQUEUE: Memproses & mengeluarkan kapal paling depan
    void processNextShip() {
        if (front == nullptr) {
            cout << "Antrean suplai kosong. Tidak ada kapal yang perlu diproses." << endl;
            return;
        }

        SupplyShip* temp = front; 
        cout << "Memproses kapal " << temp->shipName << " dengan muatan " << temp->cargoType << "..." << endl;
        
        front = front->next; 

        // Jika setelah digeser antrean jadi kosong, rear juga harus di-NULL-kan
        if (front == nullptr) {
            rear = nullptr;
        }

        delete temp;  
        saveToFile(); 
    }

    // 3. READ: Melihat daftar seluruh antrean kapal
    void displayQueue() {
        if (front == nullptr) {
            cout << "Antrean kapal suplai kosong." << endl;
            return;
        }

        cout << "\n=== ANTREAN KAPAL SUPLAI ===" << endl;
        SupplyShip* temp = front;
        int no = 1;
        while (temp != nullptr) {
            cout << no << ". Kapal: " << temp->shipName << " | Muatan: " << temp->cargoType << endl;
            temp = temp->next;
            no++;
        }
        cout << "============================\n" << endl;
    }

    // FITUR FILE I/O: Menyimpan data ke TXT
    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            SupplyShip* temp = front;
            while (temp != nullptr) {
                file << temp->shipName << " " << temp->cargoType << endl;
                temp = temp->next;
            }
            file.close();
        }
    }

    // FITUR FILE I/O: Memuat data dari TXT
    void loadFromFile() {
        ifstream file(filename);
        string name, cargo;
        
        // Membaca file dan menambahkannya ke Queue tanpa perlu print log
        while (file >> name >> cargo) {
            enqueue(name, cargo);
        }
        file.close();
    }
};



// 3. STACK: Riwayat Koordinat Kapal (Backtracking) (Prinsip LIFO: Last In First Out)
// Struct untuk Node Koordinat
struct Coordinate {
    int x, y, z;
    string eventLog; // Catatan kejadian (misal: "Menemukan_Asteroid")
    Coordinate* next;
};

// Struct untuk Sistem Navigasi (Mengelola Stack)
struct NavStackSystem {
    Coordinate* top = nullptr; 
    string filename = "riwayat_navigasi.txt";

    // 1. PUSH: Menambah koordinat baru ke ATAS tumpukan
    void push(int x, int y, int z, string log) {
        Coordinate* newCoord = new Coordinate();
        newCoord->x = x;
        newCoord->y = y;
        newCoord->z = z;
        newCoord->eventLog = log;
        
        // Node baru langsung menunjuk ke top yang lama
        newCoord->next = top; 
        
        // Top bergeser ke node yang baru
        top = newCoord;       
    }

    // Fungsi wrapper untuk interaksi user
    void addCoordinate(int x, int y, int z, string log) {
        push(x, y, z, log);
        cout << "Koordinat (" << x << "," << y << "," << z << ") dicatat: " << log << endl;
        saveToFile();
    }

    // 2. POP: Mundur ke koordinat sebelumnya (Backtracking)
    void backtrack() {
        if (top == nullptr) {
            cout << "Riwayat kosong! Kapal masih berada di stasiun utama." << endl;
            return;
        }

        Coordinate* temp = top; // Simpan posisi saat ini
        cout << "Backtracking dari titik (" << temp->x << "," << temp->y << "," << temp->z << ")..." << endl;
        
        // Geser top ke bawahnya (koordinat sebelumnya)
        top = top->next; 
        
        // Hapus koordinat yang baru saja ditinggalkan
        delete temp; 
        
        if (top != nullptr) {
            cout << "Posisi kapal sekarang di (" << top->x << "," << top->y << "," << top->z << ")" << endl;
        } else {
            cout << "Kapal telah kembali ke stasiun utama." << endl;
        }

        saveToFile();
    }

    // FUNGSI READ FILE I/O: Menampilkan dengan Rekursi
    void displayRecursive(Coordinate* node, int& step, Coordinate* topNode) {
        if (node == nullptr) return; // Base case: sudah sampai di dasar

        // 1. Menyelam terus ke tumpukan paling bawah (data tertua)
        displayRecursive(node->next, step, topNode);

        // 2. Mencetak data saat naik kembali ke atas
        cout << step << ". Posisi: (" << node->x << ", " << node->y << ", " << node->z 
             << ") | Log: " << node->eventLog;
             
        // Jika data yang sedang dicetak adalah 'top' (data terbaru), beri penanda
        if (node == topNode) {
            cout << " <-- (Posisi Saat Ini)";
        }
        cout << endl;
        
        step++; // Tambah nomor urutan
    }

    // 3. READ: Melihat seluruh riwayat perjalanan (Log Kronologis)
    void displayHistory() {
        if (top == nullptr) {
            cout << "Belum ada riwayat navigasi." << endl;
            return;
        }

        cout << "\n=== RIWAYAT NAVIGASI (Buku Log Kronologis) ===" << endl;
        int step = 1;
        // Panggil fungsi rekursif dimulai dari top
        displayRecursive(top, step, top);
        cout << "==============================================\n" << endl;
    }

    // FUNGSI FILE I/O: Menyimpan dengan Rekursi
    void saveRecursive(Coordinate* node, ofstream& file) {
        // Base case: jika sudah sampai paling dasar, berhenti menyelam
        if (node == nullptr) return; 

        // 1. Menyelam terus ke bawah sebelum menulis
        saveRecursive(node->next, file); 

        // 2. Menulis ke file saat proses rekursi naik kembali (dari dasar ke puncak)
        file << node->x << " " << node->y << " " << node->z << " " << node->eventLog << endl;
    }

    // FITUR FILE I/O: Menyimpan data ke TXT (Overwritten)
    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            saveRecursive(top, file); // Panggil fungsi rekursif dimulai dari 'top'
            file.close();
        }
    }
    // FITUR FILE I/O: Memuat data dari TXT
    void loadFromFile() {
        ifstream file(filename);
        int x, y, z;
        string log;
        while (file >> x >> y >> z >> log) {
            // Kita gunakan push manual tanpa trigger save
            push(x, y, z, log);
        }
        file.close();
    }
};

// 4. TREE: Hierarki Modul/Ruangan Stasiun (BST)
// Struct untuk Node Ruangan/Modul
struct StationModule {
    int moduleID;
    string moduleName;
    StationModule* left;  // Pointer ke cabang kiri
    StationModule* right; // Pointer ke cabang kanan
};

// Struct untuk Sistem Peta Stasiun (Mengelola BST)
struct StationMapSystem {
    StationModule* root = nullptr; // Titik pusat stasiun
    string filename = "denah_stasiun.txt";

    // 1. CREATE: Logika rekursif untuk menambah modul    
    StationModule* insertNode(StationModule* node, int id, string name) {
        // Jika menemukan tempat yang kosong, buat node baru di sini
        if (node == nullptr) {
            StationModule* newNode = new StationModule();
            newNode->moduleID = id;
            newNode->moduleName = name;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }

        // Jika ID lebih kecil, telusuri terus ke cabang KIRI
        if (id < node->moduleID) {
            node->left = insertNode(node->left, id, name);
        } 
        // Jika ID lebih besar, telusuri terus ke cabang KANAN
        else if (id > node->moduleID) {
            node->right = insertNode(node->right, id, name);
        }

        return node; 
    }

    // Fungsi wrapper agar user mudah memanggil dari main()
    void addModule(int id, string name) {
        root = insertNode(root, id, name);
        cout << "Modul [" << id << "] " << name << " ditambahkan ke denah." << endl;
        saveToFile();
    }

    // 2. READ: Menampilkan denah dengan bentuk pohon berundak (ASCII)
    void displayHierarchical(StationModule* node, int level) {
        if (node == nullptr) return;

        // 1. Cetak spasi/garis vertikal sesuai dengan kedalaman level modul
        for (int i = 0; i < level; i++) {
            cout << "    |"; 
        }
        
        // 2. Cetak tanda panah cabang dan nama modulnya
        if (level == 0) {
            cout << "--- [ROOT] "; // Garis lurus standar
        } else {
            cout << "|-- [CABANG] "; // Cabang standar
        }
        cout << "Modul [" << node->moduleID << "] " << node->moduleName << endl;

        // 3. Rekursi (panggil otomatis) untuk cabang KIRI dan KANAN
        displayHierarchical(node->left, level + 1);
        displayHierarchical(node->right, level + 1);
    }

    void displayMap() {
        if (root == nullptr) {
            cout << "Denah stasiun masih kosong." << endl;
            return;
        }
        cout << "\n======= STRUKTUR HIERARKI RUANGAN STASIUN =======" << endl;
        displayHierarchical(root, 0); // Mulai dari level 0 (root)
        cout << "=================================================" << endl;
    }

    // FITUR FILE I/O: Menyimpan data (Pre-Order Traversal)
    // Pre-order (Node - Kiri - Kanan) penting agar saat di-load, bentuk akar dan cabangnya tetap sama persis
    void savePreOrder(StationModule* node, ofstream& file) {
        if (node == nullptr) return;

        file << node->moduleID << " " << node->moduleName << endl; // Simpan node saat ini
        savePreOrder(node->left, file);  // Simpan semua di kiri
        savePreOrder(node->right, file); // Simpan semua di kanan
    }

    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            savePreOrder(root, file);
            file.close();
        }
    }

    // FITUR FILE I/O: Memuat data dari TXT
    void loadFromFile() {
        ifstream file(filename);
        int id;
        string name;
        
        // Membaca file dan membangun ulang Tree tanpa log "ditambahkan"
        while (file >> id >> name) {
            root = insertNode(root, id, name);
        }
        file.close();
    }
    
    // FITUR TAMBAHAN: Pemandu Rute dari Pusat ke Ruangan Tujuan
    void findRoute(StationModule* node, int targetID) {
        if (node == nullptr) {
            cout << "[ERROR] Sinyal terputus. Modul dengan ID " << targetID << " tidak ditemukan di denah!" << endl;
            return;
        }

        // Cetak nama ruangan yang sedang dilewati astronot
        cout << "[" << node->moduleName << "]";

        // Jika ruangan saat ini adalah tujuan akhir
        if (node->moduleID == targetID) {
            cout << " <== (Tiba di Tujuan!)" << endl;
            return;
        }

        // Jika belum sampai, gunakan sifat BST untuk menentukan arah belok
        if (targetID < node->moduleID) {
            cout << " -> Belok ke Lorong Kiri -> ";
            findRoute(node->left, targetID); 
        } else {
            cout << " -> Belok ke Lorong Kanan -> ";
            findRoute(node->right, targetID); 
        }
    }

    // Fungsi pembungkus agar mudah dipanggil di menu utama
    void guideToModule(int targetID) {
        cout << "\n=== SISTEM PEMANDU ARAH STASIUN ===" << endl;
        cout << "Menghitung rute dari Pusat menuju Modul ID " << targetID << "..." << endl;
        cout << "Rute: ";
        findRoute(root, targetID);
        cout << "===================================\n" << endl;
    }
};

// 5. HASH TABLE: Data Kru Astronot (Chaining)
// Struct untuk Node Kru (Linked List untuk Chaining)
struct CrewNode {
    string crewID;
    string name;
    string role;
    CrewNode* next; // Pointer untuk menangani collision (tabrakan)
};

const int TABLE_SIZE = 20; // Ukuran Hash Table (20 laci) 

// Struct untuk Sistem Manajemen Kru
struct CrewHashTableSystem {
    // Membuat array yang berisi pointer ke CrewNode
    CrewNode* table[TABLE_SIZE]; 
    string filename = "data_kru.txt";

    // Constructor: Mengosongkan semua laci saat program dimulai
    CrewHashTableSystem() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    // HASH FUNCTION: Mengubah ID String menjadi angka indeks
    int hashFunction(string id) {
        int sum = 0;
        // Menjumlahkan nilai ASCII dari setiap huruf di ID
        for (int i = 0; i < id.length(); i++) {
            sum += id[i];
        }
        return sum % TABLE_SIZE; 
    }

    // 1. CREATE (INSERT): Menambah kru ke Hash Table
    void insertCrew(string id, string name, string role, bool fromFile = false) {
        int index = hashFunction(id); 

        CrewNode* newCrew = new CrewNode();
        newCrew->crewID = id;
        newCrew->name = name;
        newCrew->role = role;
        newCrew->next = nullptr;

        // Jika laci kosong, langsung masukkan
        if (table[index] == nullptr) {
            table[index] = newCrew;
        } 
        // COLLISION HANDLING (Chaining): Jika laci sudah terisi, taruh di paling belakang
        else {
            CrewNode* temp = table[index];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newCrew;
        }

        if (!fromFile) {
            cout << "Kru " << name << " (ID: " << id << ") berhasil didaftarkan di laci " << index << "." << endl;
            saveToFile();
        }
    }

    // 2. READ (SEARCH): Mencari data kru secara instan berdasarkan ID
    void searchCrew(string id) {
        int index = hashFunction(id);
        CrewNode* temp = table[index];

        // Telusuri Linked List di dalam laci tersebut
        while (temp != nullptr) {
            if (temp->crewID == id) {
                cout << "\n=== DATA KRU DITEMUKAN ===" << endl;
                cout << "ID    : " << temp->crewID << endl;
                cout << "Nama  : " << temp->name << endl;
                cout << "Peran : " << temp->role << endl;
                cout << "==========================\n" << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Kru dengan ID " << id << " tidak ditemukan!" << endl;
    }

    // Menampilkan seluruh laci Hash Table (Untuk melihat proses Chaining)
    void displayTable() {
        cout << "\n=== DATABASE KRU ASTRONOT (HASH TABLE) ===" << endl;
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "Laci [" << i << "]: ";
            CrewNode* temp = table[i];
            if (temp == nullptr) {
                cout << "(Kosong)";
            }
            while (temp != nullptr) {
                cout << "{" << temp->crewID << " - " << temp->name << "} -> ";
                temp = temp->next;
            }
            cout << endl;
        }
        cout << "==========================================\n" << endl;
    }

    // FITUR FILE I/O
    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < TABLE_SIZE; i++) {
                CrewNode* temp = table[i];
                while (temp != nullptr) {
                    file << temp->crewID << " " << temp->name << " " << temp->role << endl;
                    temp = temp->next;
                }
            }
            file.close();
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        string id, name, role;
        while (file >> id >> name >> role) {
            insertCrew(id, name, role, true); // true agar tidak memicu save saat load
        }
        file.close();
    }
};

// 6. WEIGHTED GRAPH (GRAPH, SORTING, SEARCHING): Peta Navigasi Antar Planet
// 1. Struct Node Rute (Edge)
struct RouteNode {
    string destination;
    int weight; 
    RouteNode* nextRoute;
};

// 2. Struct Node Lokasi (Vertex)
struct LocationNode {
    string name;
    RouteNode* routeHead; 
    LocationNode* nextLoc; 
};

// 3. Struct Utama Sistem Peta (Weighted Graph (Graph + Sorting + Searching))
struct WeightedGraphSystem {
    LocationNode* head = nullptr; 
    string filename = "peta_navigasi.txt";

    // 3.1 Fungsi untuk Mencari Lokasi berdasarkan Nama
    LocationNode* findLocation(string name) {
        LocationNode* temp = head;
        while (temp != nullptr) {
            if (temp->name == name) return temp;
            temp = temp->nextLoc;
        }
        return nullptr;
    }

    // 3.2 Fungsi File I/O Save (dipakai saat add/sort)
    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) {
            LocationNode* tempLoc = head;
            while (tempLoc != nullptr) {
                file << "LOC " << tempLoc->name << endl;
                tempLoc = tempLoc->nextLoc;
            }
            tempLoc = head;
            while (tempLoc != nullptr) {
                RouteNode* tempRoute = tempLoc->routeHead;
                while (tempRoute != nullptr) {
                    file << "ROUTE " << tempLoc->name << " " << tempRoute->destination << " " << tempRoute->weight << endl;
                    tempRoute = tempRoute->nextRoute;
                }
                tempLoc = tempLoc->nextLoc;
            }
            file.close();
        }
    }

    // 3.3 Fungsi-fungsi Manajemen Graph (addLocation, addRoute, load)
    void addLocation(string name, bool fromFile = false) {
        if (findLocation(name) != nullptr) return;

        LocationNode* newLoc = new LocationNode();
        newLoc->name = name;
        newLoc->routeHead = nullptr;
        newLoc->nextLoc = nullptr;

        if (head == nullptr) {
            head = newLoc;
        } else {
            LocationNode* temp = head;
            while (temp->nextLoc != nullptr) temp = temp->nextLoc;
            temp->nextLoc = newLoc;
        }

        if (!fromFile) {
            cout << "Titik [" << name << "] ditambahkan." << endl;
            saveToFile();
        }
    }

    void addRoute(string source, string dest, int weight, bool fromFile = false) {
        LocationNode* srcNode = findLocation(source);
        LocationNode* destNode = findLocation(dest);

        if (srcNode == nullptr || destNode == nullptr) return;

        RouteNode* newRoute = new RouteNode();
        newRoute->destination = dest;
        newRoute->weight = weight;
        newRoute->nextRoute = nullptr;

        if (srcNode->routeHead == nullptr) {
            srcNode->routeHead = newRoute;
        } else {
            RouteNode* temp = srcNode->routeHead;
            while (temp->nextRoute != nullptr) temp = temp->nextRoute;
            temp->nextRoute = newRoute;
        }

        if (!fromFile) {
            cout << "Rute dibuka: " << source << " -> " << dest << endl;
            saveToFile();
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        string type, param1, param2;
        int weight;
        while (file >> type) {
            if (type == "LOC") {
                file >> param1;
                addLocation(param1, true);
            } else if (type == "ROUTE") {
                file >> param1 >> param2 >> weight;
                addRoute(param1, param2, weight, true);
            }
        }
        file.close();
    }

    void displayMap() {
        LocationNode* tempLoc = head;
        while (tempLoc != nullptr) {
            cout << "[ " << tempLoc->name << " ] -> ";
            RouteNode* tempRoute = tempLoc->routeHead;
            while (tempRoute != nullptr) {
                cout << tempRoute->destination << "(" << tempRoute->weight << " TC) ";
                tempRoute = tempRoute->nextRoute;
            }
            cout << endl;
            tempLoc = tempLoc->nextLoc;
        }
    }

    // 3.4 Logika Manual MERGE SORT (Sorting)
    RouteNode* sortedMerge(RouteNode* a, RouteNode* b) {
        RouteNode* result = nullptr;
        if (a == nullptr) return b;
        else if (b == nullptr) return a;

        if (a->weight <= b->weight) {
            result = a;
            result->nextRoute = sortedMerge(a->nextRoute, b);
        } else {
            result = b;
            result->nextRoute = sortedMerge(a, b->nextRoute);
        }
        return result;
    }

    void splitList(RouteNode* source, RouteNode** frontRef, RouteNode** backRef) {
        RouteNode* fast;
        RouteNode* slow;
        slow = source;
        fast = source->nextRoute;
        while (fast != nullptr) {
            fast = fast->nextRoute;
            if (fast != nullptr) {
                slow = slow->nextRoute;
                fast = fast->nextRoute;
            }
        }
        *frontRef = source;
        *backRef = slow->nextRoute;
        slow->nextRoute = nullptr;
    }

    void mergeSortRoutes(RouteNode** headRef) {
        RouteNode* head = *headRef;
        RouteNode* a;
        RouteNode* b;
        if ((head == nullptr) || (head->nextRoute == nullptr)) return;

        splitList(head, &a, &b);
        mergeSortRoutes(&a);
        mergeSortRoutes(&b);
        *headRef = sortedMerge(a, b);
    }

    // 3.5 Fitur Navigasi Terpadu (Searching)
    void searchSpecificRoute(string source, string targetDest) {
        LocationNode* loc = findLocation(source);
        if (loc == nullptr) {
            cout << "Titik awal " << source << " tidak ditemukan!" << endl;
            return;
        }

        RouteNode* temp = loc->routeHead;
        bool found = false;
        
        cout << "\nMencari rute dari " << source << " menuju " << targetDest << "..." << endl;
        while (temp != nullptr) {
            if (temp->destination == targetDest) {
                cout << "-> RUTE DITEMUKAN! Jarak/Bobot: " << temp->weight << " Tahun Cahaya." << endl;
                found = true;
                break;
            }
            temp = temp->nextRoute;
        }

        if (!found) {
            cout << "-> Rute tidak ditemukan! Belum ada jalur langsung ke " << targetDest << "." << endl;
        }
    }
    void findMostEfficientRoute(string locationName) {
        LocationNode* loc = findLocation(locationName); 
        if (loc == nullptr || loc->routeHead == nullptr) {
            cout << "Tidak ada rute tersedia." << endl;
            return;
        }

        mergeSortRoutes(&(loc->routeHead)); // Lakukan sorting dahulu

        RouteNode* optimalRoute = loc->routeHead; // Data pertama adalah yang terpendek
        cout << "\n=== REKOMENDASI RUTE PALING EFISIEN DARI " << locationName << " ===" << endl;
        cout << "Tujuan Optimal : " << optimalRoute->destination << endl;
        cout << "Jarak Tempuh   : " << optimalRoute->weight << " Tahun Cahaya" << endl;
        cout << "Status         : Aman untuk Hyperjump" << endl;
        cout << "========================================================\n" << endl;
    }
};
// ==========================================
// FUNGSI MAIN (CLI UTAMA)
// ==========================================
int main() {
    // INISIALISASI SEMUA SISTEM STRUKTUR DATA
    InventorySystem inv;           // Linked List
    SupplyQueueSystem queueSys;    // Queue
    NavStackSystem navSys;         // Stack
    StationMapSystem treeSys;      // Tree
    CrewHashTableSystem crewSys;   // Hash Table
    WeightedGraphSystem mapSys;    // Graph + Sort + Search

    // MEMUAT DATA SECARA OTOMATIS DARI FILE TXT
    inv.loadFromFile();
    queueSys.loadFromFile();
    navSys.loadFromFile();
    treeSys.loadFromFile();
    crewSys.loadFromFile();
    mapSys.loadFromFile();

    int menuUtama, subMenu;
    string strParam1, strParam2, strParam3;
    int intParam1, intParam2, intParam3;

    do {
        cout << "\n=====================================================" << endl;
        cout << "      SISTEM KONTROL PUSAT KOLONI LUAR ANGKASA       " << endl;
        cout << "=====================================================" << endl;
        cout << "1. Manajemen Data Kru Astronot       (Hash Table)" << endl;
        cout << "2. Denah Modul / Ruangan Stasiun     (Tree/BST)" << endl;
        cout << "3. Antrean Kapal Suplai Logistik     (Queue)" << endl;
        cout << "4. Inventaris Suku Cadang Kapal      (Linked List)" << endl;
        cout << "5. Riwayat Navigasi Kapal Patroli    (Stack)" << endl;
        cout << "6. Peta Navigasi & Alur Kosmik       (Weighted Graph)" << endl;
        cout << "7. Keluar dari Sistem" << endl;
        cout << "Pilih menu [1-7]: ";
        cin >> menuUtama;

        switch (menuUtama) {
            case 1: // HASH TABLE - DATA KRU
                cout << "\n--- MANAJEMEN DATA KRU ASTRONOT ---" << endl;
                cout << "1. Registrasi Kru Baru\n2. Cari Data Kru\n3. Tampilkan Semua Laci Database\n0. Kembali ke Menu Utama\nPilih opsi: ";
                cin >> subMenu;
                if (subMenu == 1) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan ID Kru (misal: CMD01): "; cin >> strParam1;
                    cout << "Masukkan Nama Kru (gunakan _ ): "; cin >> strParam2;
                    cout << "Masukkan Peran / Jabatan       : "; cin >> strParam3;
                    cout << "------------------------------------------------------"<< endl;
                    crewSys.insertCrew(strParam1, strParam2, strParam3);
                } else if (subMenu == 2) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan ID Kru yang dicari: "; cin >> strParam1;
                    cout << "------------------------------------------------------"<< endl;
                    crewSys.searchCrew(strParam1);
                } else if (subMenu == 3) {
                    crewSys.displayTable();
                }
                else if (subMenu == 0) { 
                    cout << "Kembali ke layar utama..." << endl; }
                break;

            case 2: // TREE - DENAH RUANGAN
                cout << "\n--- DENAH MODUL STASIUN ---" << endl;
                cout << "1. Tambah Modul Ruangan Baru\n2. Tampilkan Hierarki Denah\n3. Panduan Rute ke Ruangan\n0. Kembali ke Menu Utama\nPilih opsi: ";
                cin >> subMenu;
                if (subMenu == 1) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan ID Modul (Angka Unik): "; cin >> intParam1;
                    cout << "Masukkan Nama Modul (gunakan _): "; cin >> strParam1;
                    cout << "------------------------------------------------------"<< endl;
                    treeSys.addModule(intParam1, strParam1);
                } else if (subMenu == 2) {
                    treeSys.displayMap();
                } else if (subMenu == 3) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan ID Modul Tujuan yang dicari: "; cin >> intParam1;
                    cout << "------------------------------------------------------"<< endl;
                    treeSys.guideToModule(intParam1);    
                } else if (subMenu == 0) {
                    cout << "Kembali ke layar utama..." << endl;
                }
                break;

            case 3: // QUEUE - ANTREAN LOGISTIK
                cout << "\n--- ANTREAN KAPAL SUPLAI LOGISTIK ---" << endl;
                cout << "1. Daftarkan Kapal Masuk Antrean\n2. Proses Kapal Terdepan\n3. Lihat Daftar Antrean\n0. Kembali ke Menu Utama\nPilih opsi: ";
                cin >> subMenu;
                if (subMenu == 1) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Kapal (gunakan _): "; cin >> strParam1;
                    cout << "Masukkan Jenis Muatan          : "; cin >> strParam2;
                    cout << "------------------------------------------------------"<< endl;
                    queueSys.addShip(strParam1, strParam2);
                } else if (subMenu == 2) {
                    queueSys.processNextShip();
                } else if (subMenu == 3) {
                    queueSys.displayQueue();
                } else if (subMenu == 0) {
                    cout << "Kembali ke layar utama..." << endl;
                }
                break;

            case 4: // LINKED LIST - INVENTARIS
                cout << "\n--- INVENTARIS SUKU CADANG ---" << endl;
                cout << "1. Tambah Barang Baru\n2. Update Jumlah Stok\n3. Hapus Barang\n4. Tampilkan Inventaris\n0. Kembali ke Menu Utama\nPilih opsi: ";
                cin >> subMenu;
                if (subMenu == 1) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Barang (gunakan _): "; cin >> strParam1;
                    cout << "Masukkan Jumlah Stok            : "; cin >> intParam1;
                    cout << "------------------------------------------------------"<< endl;
                    inv.addPart(strParam1, intParam1);
                    inv.saveToFile();
                    cout << "Barang berhasil ditambahkan." << endl;
                } else if (subMenu == 2) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Barang yang diupdate: "; cin >> strParam1;
                    cout << "Masukkan Jumlah Stok Baru         : "; cin >> intParam1;
                    cout << "------------------------------------------------------"<< endl;
                    inv.updatePart(strParam1, intParam1);
                } else if (subMenu == 3) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Barang yang dihapus: "; cin >> strParam1;
                    cout << "------------------------------------------------------"<< endl;
                    inv.deletePart(strParam1);
                } else if (subMenu == 4) {
                    inv.displayInventory();
                } else if (subMenu == 0) {
                    cout << "Kembali ke layar utama..." << endl;
                }
                break;

            case 5: // STACK - RIWAYAT NAVIGASI KAPAL PATROLI
                cout << "\n--- RIWAYAT NAVIGASI KAPAL PATROLI ---" << endl;
                cout << "1. Catat Koordinat Baru (Bergerak Maju)\n2. Backtracking (Mundur ke Titik Sebelumnya)\n3. Lihat Semua Riwayat Perjalanan\n0. Kembali ke Menu Utama\nPilih opsi: ";
                cin >> subMenu;
                if (subMenu == 1) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Koordinat X Y Z (pisahkan spasi): "; cin >> intParam1 >> intParam2 >> intParam3;
                    cout << "Masukkan Catatan Kejadian (gunakan _): "; cin >> strParam1;
                    cout << "------------------------------------------------------"<< endl;
                    navSys.addCoordinate(intParam1, intParam2, intParam3, strParam1);
                } else if (subMenu == 2) {
                    navSys.backtrack();
                } else if (subMenu == 3) {
                    navSys.displayHistory();
                } else if (subMenu == 0) {
                    cout << "Kembali ke layar utama..." << endl;
                }
                break;

            case 6: // GRAPH + SORTING + SEARCHING - PETA NAVIGASI KOSMIK
                cout << "\n--- PETA NAVIGASI KOSMIK ---" << endl;
                cout << "1. Tambah Titik Lokasi Baru (Planet/Stasiun)\n2. Buka Rute Baru Antar Lokasi (Edge)\n3. Tampilkan Peta Navigasi\n4. Cari Jalur Paling Efisien (Merge-Searching)\n5. Cari Rute Spesifik\n0. Kembali ke Menu Utama\nPilih opsi: ";
                cin >> subMenu;
                if (subMenu == 1) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Lokasi Baru: "; cin >> strParam1;
                    cout << "------------------------------------------------------"<< endl;
                    mapSys.addLocation(strParam1);
                } else if (subMenu == 2) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Lokasi Asal   : "; cin >> strParam1;
                    cout << "Masukkan Nama Lokasi Tujuan : "; cin >> strParam2;
                    cout << "Masukkan Jarak (Tahun Cahaya): "; cin >> intParam1;
                    cout << "------------------------------------------------------"<< endl;
                    mapSys.addRoute(strParam1, strParam2, intParam1);
                } else if (subMenu == 3) {
                    mapSys.displayMap();
                } else if (subMenu == 4) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Nama Lokasi Asal Kamu: "; cin >> strParam1;
                    cout << "------------------------------------------------------"<< endl;
                    mapSys.findMostEfficientRoute(strParam1);
                } else if (subMenu == 5) {
                    cout << "------------------------------------------------------"<< endl;
                    cout << "Masukkan Lokasi Asal   : "; cin >> strParam1;
                    cout << "Masukkan Lokasi Tujuan : "; cin >> strParam2;
                    cout << "------------------------------------------------------"<< endl;
                    mapSys.searchSpecificRoute(strParam1, strParam2);   
                } else if (subMenu == 0) {
                    cout << "Kembali ke layar utama..." << endl;
                }
                break;

            case 7:
                cout << "\nMematikan sistem kontrol..." << endl;
                cout << "Menyimpan perubahan seluruh data ke dalam file TXT..." << endl;
                
                // Melakukan proses Mass Save untuk semua struktur data
                inv.saveToFile();
                queueSys.saveToFile();
                navSys.saveToFile();
                treeSys.saveToFile();
                crewSys.saveToFile();
                mapSys.saveToFile();

                cout << "[OK] Data Inventaris diamankan." << endl;
                cout << "[OK] Data Antrean diamankan." << endl;
                cout << "[OK] Riwayat Navigasi diamankan." << endl;
                cout << "[OK] Denah Stasiun diamankan." << endl;
                cout << "[OK] Database Kru diamankan." << endl;
                cout << "[OK] Peta Peta Navigasi Kosmik diamankan." << endl;
                
                cout << "\nSistem Offline. Sampai jumpa di misi berikutnya, Komandan!" << endl;
                break;
                
            default:
                cout << "Pilihan salah! Sinyal perintah tidak dikenali komputer stasiun." << endl;
        }
    } while (menuUtama != 7);

    return 0;
}