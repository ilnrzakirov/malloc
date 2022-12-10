# malloc

Этот проект посвящен реализации механизма динамического распределения памяти.


mmap - отображает файла в адресное пространство
<img alt="img" width="400px"  align="center" src="https://github.com/ilnrzakirov/malloc/blob/master/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202022-12-09%20%D0%B2%2018.15.20.png"> <br>

Приницип работы малока на изображении ниже, в первый блок (header) записываем размер выделяемой памяти (size + header size). 

<img alt="img" width="400px"  align="center" src="https://github.com/ilnrzakirov/malloc/blob/master/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%202022-12-10%20%D0%B2%2012.50.17.png">
