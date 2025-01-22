#include <iostream>
#include <string>
#include <unordered_map>


// 信号调制类型 //
enum SignalModulationType {
  SMT_UNKNOWN = 0,
  SMT_NOISE,
  SMT_MSK,
  SMT_GMSK,
  SMT_2FSK,
  SMT_BPSK,
  SMT_QPSK,
  SMT_8PSK,
  SMT_OQPSK,
  SMT_PI4DQPSK,
  SMT_16QAM,
  SMT_32QAM,
  SMT_64QAM,
  SMT_128QAM,
  SMT_256QAM,
  SMT_512QAM,
};


std::unordered_map<int, std::string> umap_input_uint8 = {
    {SMT_UNKNOWN, "UNKNOWN" },
    {SMT_NOISE, "NOISE" },
    {SMT_MSK, "MSK" },
    {SMT_GMSK, "GMSK" },
    {SMT_2FSK, "2FSK" },
    {SMT_BPSK, "BPSK" },
    {SMT_QPSK, "QPSK" },
    {SMT_8PSK, "8PSK" },
    {SMT_OQPSK, "OQPSK" },
    {SMT_PI4DQPSK, "PI4DQPSK" },
    {SMT_16QAM, "16QAM" },
    {SMT_32QAM, "32QAM" },
    {SMT_64QAM, "64QAM" },
    {SMT_128QAM, "128QAM" },
    {SMT_256QAM, "256QAM" },
    {SMT_512QAM, "512QAM" }
};

std::unordered_map<std::string, int> umap_input_string = {
    {"UNKNOWN", SMT_UNKNOWN },
    {"NOISE", SMT_NOISE },
    {"MSK", SMT_MSK },
    {"GMSK", SMT_GMSK },
    {"2FSK", SMT_2FSK },
    {"BPSK", SMT_BPSK },
    {"QPSK", SMT_QPSK },
    {"8PSK", SMT_8PSK },
    {"OQPSK", SMT_OQPSK },
    {"PI4DQPSK", SMT_PI4DQPSK },
    {"16QAM", SMT_16QAM },
    {"32QAM", SMT_32QAM },
    {"64QAM", SMT_64QAM },
    {"128QAM", SMT_128QAM },
    {"256QAM", SMT_256QAM },
    {"512QAM", SMT_512QAM }
};

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 16; i ++) {
        printf("%d: %s\n", i, umap_input_uint8[i].data());
    }

    for (int i = 0; i < 16; i ++) {
        printf("%s: %d\n", umap_input_uint8[i].data(), umap_input_string[umap_input_uint8[i].data()]);
    }
    return 0;
}
