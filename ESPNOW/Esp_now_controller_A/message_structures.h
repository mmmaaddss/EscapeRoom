
// worker - printer
typedef struct worker_printer_struct {
  bool game_running;
  char phonenumber[9];
  char name[100];
  char postnummer[5];
  bool print_numbers;
} worker_printer_struct;

// worker - numpad
typedef struct worker_numpad_struct {
  bool game_running;
  char postnummer[5];
} worker_numpad_struct;

// worker - Telefon
typedef struct worker_telefon_struct {
  bool game_running;
  char phonenumber[9];
} worker_telefon_struct;

// worker - elboks
typedef struct worker_elboks_struct {
  bool game_running;
} worker_elboks_struct;


// incomming worker message
typedef struct incomming_worker_message {
  bool completed;
} incomming_worker_message;