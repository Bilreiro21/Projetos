from pessoa import Pessoa

# Criação de instâncias da classe Pessoa com nome e data de nascimento
joao = Pessoa("João")
joao.setDataNascimento(2002, 4, 14)

marta = Pessoa("Marta")
marta.setDataNascimento(2005, 9, 18)

ronaldo = Pessoa("Ronaldo")
ronaldo.setDataNascimento(1985, 2, 5)

# Lista de convidados
lista_convidados = []

# Adiciona as pessoas à lista
lista_convidados.append(joao)
lista_convidados.append(marta)
lista_convidados.append(ronaldo)

# Imprime apenas os nomes dos convidados
for pessoa in lista_convidados:
    print(pessoa.getNome())
