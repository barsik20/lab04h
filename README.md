# Лабораторная работа №4
Целью данной лабораторной работы являлась настройка непрерывной интеграции (CI) для библиотек и приложений из лабораторной работы №3. Вместо Travis CI был использован GitHub Actions. В рамках работы были настроены автоматические сборки при каждом push и pull request в ветки main/master. Сборочные процедуры реализованы для двух платформ:
1) Linux (Ubuntu latest) — с компиляторами GCC и Clang</br>
2) Windows (Windows latest) — с компилятором Clang</br>
Код программы:
```
name: CI  

on:  
  push:  
    branches: [ main, master ] 
  pull_request: 
    branches: [ main, master ]  

jobs: 
  build: 
  
    runs-on: ${{ matrix.os }}  # Берётся из матрицы ниже
    # ${{ ... }} — подстановка значения переменной
    
    strategy:
      matrix:
        # Перебор операционных систем
        os: [ ubuntu-latest, windows-latest ]
        
        # Перебор компиляторов
        compiler:
          - cxx: g++       
            cc: gcc        
          - cxx: clang++   
            cc: clang      
        
        # ИСКЛЮЧЕНИЕ НЕНУЖНЫХ КОМБИНАЦИЙ
        exclude:
          - os: windows-latest      # Исключаем комбинацию:
            compiler:
              cxx: g++             
              cc: gcc
        # На Windows сложно установить GCC. а Clang устанавливается проще через Chocolatey
    steps:
    
      - uses: actions/checkout@v5

      - name: Install dependencies (Ubuntu)  # Название шага в логах
        if: runner.os == 'Linux' 
        run: |  
          sudo apt-get update              
          sudo apt-get install -y cmake build-essential

      - name: Install Clang (Ubuntu)  
        if: runner.os == 'Linux' && matrix.compiler.cxx == 'clang++'
        run: sudo apt-get install -y clang  
        # Для GCC этот шаг пропускается (gcc уже в build-essential)


      - name: Install Clang (Windows) 
        if: runner.os == 'Windows'  # ТОЛЬКО на Windows
        shell: pwsh  # Использовать PowerShell (родной для Windows)
        run: |  # Выполнить команды PowerShell
          choco install llvm -y
          # choco        — Chocolatey, пакетный менеджер Windows
          # install llvm — установить LLVM (включает clang, clang++)
          # -y           — автоматически подтвердить установку
          
          echo "C:\Program Files\LLVM\bin" | Out-File -FilePath $env:GITHUB_PATH -Encoding utf8 -Append
          # echo "путь"                           — вывести путь к папке с clang.exe
          # |                                      — передать вывод следующей команде
          # Out-File                               — записать в файл
          # -FilePath $env:GITHUB_PATH            — путь к файлу GITHUB_PATH
          # -Encoding utf8                         — кодировка UTF-8
          # -Append                                — добавить в конец файла

      - name: Configure CMake  # Название шага
        shell: bash  
        run: cmake -B build -DCMAKE_CXX_COMPILER=${{ matrix.compiler.cxx }} -DCMAKE_C_COMPILER=${{ matrix.compiler.cc }}
        # cmake -B build                       — создать папку build для сборки
        # -DCMAKE_CXX_COMPILER=значение        — указать компилятор C++
        # -DCMAKE_C_COMPILER=значение          — указать компилятор C
        # ${{ matrix.compiler.cxx }}           — подставится g++ или clang++
        # ${{ matrix.compiler.cc }}            — подставится gcc или clang
        # ВСЁ В ОДНУ СТРОКУ — чтобы избежать проблем с переносами строки (\)

      - name: Build  
        shell: bash  
        run: cmake --build build


      - name: Run hello_world_app  
        shell: bash  
        run: |  
          if [ "${{ matrix.os }}" = "windows-latest" ]; then
            # ЕСЛИ ОС = Windows:
            find build -name "hello_world*.exe" -type f -exec {} \;
          else
            # ИНАЧЕ (Linux):
            find build -name "hello_world*" -type f -executable -exec {} \;
          fi  # Конец условия if

      - name: Run solver_app  
        shell: bash  
        run: |  
          if [ "${{ matrix.os }}" = "windows-latest" ]; then
            # ЕСЛИ Windows:
            find build -name "solver*.exe" -type f -exec {} \;
          else
            # ИНАЧЕ (Linux):
            find build -name "solver*" -type f -executable -exec {} \;
          fi  # Конец условия
```
