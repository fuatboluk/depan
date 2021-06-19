Türkiye için deprem bilgilerini terminal üzerinden çıktılayan basit bir program. Curl ile çalışır ve internete bağlı olmayı gerektirir.

Bağımlılıkları:
  libcurl

Derleme için klonladığınız proje dizinine geçin.

Derleme:

gcc -o depan depan.c -libcurl

Oluşan depan adlı programı /usr/bin dizini içerisine taşıyın. Bulunduğunuz dizinde çalıştırmak için:

./depan

Yazılım, http://www.koeri.boun.edu.tr 'ye bir curl isteği atarak html çıktı içerisinden verileri ayıklar 

ve biçimlendirme uyguladıktan sonra metin çıktısı olarak ekrana basar.

Ayrıca terminal üzerinde, head, tail ve grep kullanarak çıktıyı biçimlendirebilirsiniz.

Örnekler:

depan | head -n 10    -    ilk 10 satırı verir.

depan | tail -n 10    -    son 10 satırı verir.

depan | grep ISTANBUL -    500 satırlık çıktıda, içinde ISTANBUL geçen satırları basar.

Bu komutları saniyede bir işleyecek şekilde bir döngü içerisinde çalıştırarak basit bir anlık monitör oluşturabilirsiniz.
