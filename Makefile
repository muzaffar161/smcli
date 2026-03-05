# SmCLI Makefile

CXX = g++
CXXFLAGS = -std=c++17 -I./include -Wall -Wextra -O3
SRCS = $(wildcard src/*.cpp)
OBJ_DIR = obj
OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)/%.o)
TARGET = bin/smcli

# Installation paths
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
COMPDIR = /etc/bash_completion.d

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful: $(TARGET)"

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) bin/

install: all
	@echo "Installing binary to $(DESTDIR)$(BINDIR)"
	@mkdir -p $(DESTDIR)$(BINDIR)
	@cp $(TARGET) $(DESTDIR)$(BINDIR)/
	@chmod 755 $(DESTDIR)$(BINDIR)/smcli
	@echo "Installing bash completion..."
	@if [ -d "/etc/bash_completion.d" ]; then \
		cp completions/smcli-completion.sh /etc/bash_completion.d/smcli; \
	elif [ -d "/usr/share/bash-completion/completions" ]; then \
		cp completions/smcli-completion.sh /usr/share/bash-completion/completions/smcli; \
	fi
	@echo "Installation complete!"

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/smcli
	rm -f /etc/bash_completion.d/smcli
	rm -f /usr/share/bash-completion/completions/smcli
	@echo "Uninstalled successfully."
