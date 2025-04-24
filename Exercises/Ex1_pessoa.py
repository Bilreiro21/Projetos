# Definição da classe Pessoa
class Pessoa:
    
    # Método construtor que define o nome e inicializa a data de nascimento como None
    def __init__(self, nome="Anonimo"):
        self.__nome = nome  # Atributo privado nome
        self.__data_nascimento = None  # Atributo privado data de nascimento

    # Método para definir a data de nascimento
    def setDataNascimento(self, ano, mes, dia):
        self.__data_nascimento = (ano, mes, dia)  # Guarda a data como uma tupla

    # Método para obter a data de nascimento
    def getDataNascimento(self):
        return self.__data_nascimento  # Retorna a data de nascimento

    # Método para obter o nome
    def getNome(self):
        return self.__nome

    # Método para alterar o nome
    def setNome(self, nome):
        self.__nome = nome
