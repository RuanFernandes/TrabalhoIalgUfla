#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std;

const char* arquivoDados = "dados.bin";
const int tamanhoMaximo = 100;

struct Registro {
    int identificador;
    string nome;
    string inventor;
    int anoLancamento;
    float popularidade;
};

int lerDadosDoArquivo(Registro dados[], int tamanho) {
    ifstream arquivo(arquivoDados, ios::binary);
    int contador = 0;

    if (arquivo.is_open()) {
        while (contador < tamanho && arquivo.read(reinterpret_cast<char*>(&dados[contador]), sizeof(Registro))) {
            contador++;
        }

        arquivo.close();
    }

    cout << "Registros lidos do arquivo: " << contador << endl; // Adição para depuração

    return contador;
}



void escreverDadosNoArquivo(const Registro dados[], int tamanho) {
    ofstream arquivo(arquivoDados, ios::binary);

    for (int i = 0; i < tamanho; i++) {
        arquivo.write(reinterpret_cast<const char*>(&dados[i]), sizeof(Registro));
    }

    arquivo.close();
}

void ordenarPorNome(Registro dados[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (dados[j].nome > dados[j + 1].nome) {
                swap(dados[j], dados[j + 1]);
            }
        }
    }
}

int buscarPorNome(const Registro dados[], int tamanho, const string& nomeBusca) {
    for (int i = 0; i < tamanho; i++) {
        if (dados[i].nome == nomeBusca) {
            return i;
        }
    }
    return -1;
}

void imprimirRegistros(const Registro dados[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        cout << "ID: " << dados[i].identificador << ", Nome: " << dados[i].nome
             << ", Inventor: " << dados[i].inventor
             << ", Ano de Lancamento: " << dados[i].anoLancamento
             << ", Popularidade: " << dados[i].popularidade << endl;
    }
}


void adicionarRegistro(Registro dados[], int& tamanhoAtual) {
    if (tamanhoAtual < tamanhoMaximo) {
        cout << "Digite os dados do novo registro:\n";
        cout << "Nome: ";
        cin.ignore();
        getline(cin, dados[tamanhoAtual].nome);
        cout << "Inventor: ";
        getline(cin, dados[tamanhoAtual].inventor);
        cout << "Ano de Lancamento: ";
        cin >> dados[tamanhoAtual].anoLancamento;
        cout << "Popularidade: ";
        cin >> dados[tamanhoAtual].popularidade;

        dados[tamanhoAtual].identificador = tamanhoAtual + 1; // Ajuste conforme necessário

        tamanhoAtual++;
    } else {
        cout << "Limite de registros atingido.\n";
    }
}

void importarCSV(Registro dados[], int& tamanhoAtual) {
    // Nome do arquivo binário
    const char* arquivoBinario = "dados.bin";

    // Exclui o arquivo binário existente
    remove(arquivoBinario);

    // Cria um novo arquivo binário
    ofstream novoArquivo(arquivoBinario, ios::binary);
    novoArquivo.close();

    string nomeArquivoCSV;
    cout << "Digite o nome do arquivo CSV a ser importado (separador ';'): ";
    cin >> nomeArquivoCSV;

    ifstream arquivoCSV(nomeArquivoCSV);
    if (!arquivoCSV.is_open()) {
        cout << "Erro ao abrir o arquivo CSV.\n";
        return;
    }

    // Ignora a primeira linha do arquivo CSV que contém os cabeçalhos
    string cabecalhos;
    getline(arquivoCSV, cabecalhos);

    while (arquivoCSV.good() && tamanhoAtual < tamanhoMaximo) {
        char delim;
        arquivoCSV >> dados[tamanhoAtual].identificador;
        arquivoCSV >> delim; // Ler o separador ;
        getline(arquivoCSV, dados[tamanhoAtual].nome, ';');
        getline(arquivoCSV, dados[tamanhoAtual].inventor, ';');
        arquivoCSV >> dados[tamanhoAtual].anoLancamento;
        arquivoCSV >> delim; // Ler o separador ;
        arquivoCSV >> dados[tamanhoAtual].popularidade;

        tamanhoAtual++;
    }

    if (arquivoCSV.eof()) {
        cout << "Importacao concluida.\n";
    } else {
        cout << "Erro durante a importacao. Verifique o formato do arquivo CSV.\n";
    }

    arquivoCSV.close();
}

int main() {
    // Verifica se o arquivo binário existe, caso contrário, cria um novo
    ifstream arquivoExistente(arquivoDados);
    if (!arquivoExistente.is_open()) {
        ofstream novoArquivo(arquivoDados, ios::binary);
        novoArquivo.close();
    }
    arquivoExistente.close();

    Registro dados[tamanhoMaximo];
    int tamanhoAtual = lerDadosDoArquivo(dados, tamanhoMaximo);

    int opcao;

    while (true) {
        cout << "\nEscolha uma opcao:\n";
        cout << "1. Ordenar por nome\n";
        cout << "2. Buscar por nome\n";
        cout << "3. Imprimir registros\n";
        cout << "4. Adicionar registro\n";
        cout << "5. Importar CSV\n";
        cout << "6. Sair\n";
        cout << "Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                ordenarPorNome(dados, tamanhoAtual);
                escreverDadosNoArquivo(dados, tamanhoAtual);
                cout << "Registros ordenados por nome.\n";
                break;
            case 2: {
                string nomeBusca;
                cout << "Digite o nome a ser buscado: ";
                cin.ignore(); // Limpa o buffer do teclado
                getline(cin, nomeBusca);
                int indiceEncontrado = buscarPorNome(dados, tamanhoAtual, nomeBusca);
                if (indiceEncontrado != -1) {
                    cout << "Registro encontrado: " << dados[indiceEncontrado].identificador << " - " << dados[indiceEncontrado].nome << endl;
                } else {
                    cout << "Registro nao encontrado." << endl;
                }
                break;
            }
            case 3:
                imprimirRegistros(dados, tamanhoAtual);
                break;
            case 4:
                adicionarRegistro(dados, tamanhoAtual);
                escreverDadosNoArquivo(dados, tamanhoAtual);
                break;
            case 5:
                importarCSV(dados, tamanhoAtual);
                escreverDadosNoArquivo(dados, tamanhoAtual);
                break;
            case 6:
                cout << "Saindo do programa.\n";
                return 0;
            default:
                cout << "Opcao invalida. Tente novamente.\n";
        }
    }

    return 0;
}
