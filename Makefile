# Định nghĩa biến
CXX = g++
CXXFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0`
SRC = testgtk.cpp
TARGET = testgtk

# Quy tắc để build tất cả
all: $(TARGET)

# Quy tắc để build tệp thực thi
$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(CXXFLAGS) $(LIBS)

# Quy tắc để clean các tệp sinh ra
clean:
	rm -f $(TARGET)
