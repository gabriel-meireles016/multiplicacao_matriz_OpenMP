# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -O3 -std=c++11 -fopenmp

# Diretórios
SRCDIR = src
OBJDIR = obj

# Encontra todos os arquivos .cpp no diretório src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Gera os nomes dos arquivos objeto
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%)

# Regra principal
all: $(OBJECTS)

# Regra para compilar cada arquivo objeto
$(OBJDIR)/%: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Cria o diretório obj se não existir
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Limpeza
clean:
	rm -rf $(OBJDIR)

.PHONY: all clean list