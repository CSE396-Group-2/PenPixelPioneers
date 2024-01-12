# Pen Pixel Pioneers

# PenPixelPioneers Project

## Linux Build Dosyası Çalıştırma Talimatları

### Donanım Bağlantısı

- **Güç Kablosu:** 12V 5A
- Donanım güç kaynağına bağlandıktan sonra Arduino'yu bilgisayarınıza bağlamak yeterlidir.

### Proje Gereksinimleri

- opencv
- fontforge
- potrace
- python3

### OpenCV Kurulumu

Eğer Linux Ubuntu üzerinde OpenCV yüklü değilse, aşağıdaki adımları takip edebilirsiniz:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install libjpeg-dev libpng-dev libtiff-dev libv4l-dev libxvidcore-dev libx264-dev libatlas-base-dev gfortran
git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
```
Eğer Linux Ubuntunuzda python3 yoksa aşağıdaki adımları izleyerek kurabilirsiniz:

```bash
sudo apt-get update
sudo apt-get install python3
```
Kurulumu kontrol etmek için aşağıdaki komutu çalıştırmanız yeterli:
```bash
python3 --version
```

Aşağıda yer alan komutlar ile gerekli kütüphaneleri kurabilirsiniz:

```bash
sudo apt-get install potrace
sudo apt-get install fontforge
sudo apt-get install python3-fontforge
pip install numpy
pip install opencv-python
pip install imutils
pip install scikit-image
```

Gönderdiğimiz PenPixelPioneers klasörü içerisindeki bash dosyası çalıştırılmalıdır.

```bash
./project.sh
```

Paragrafı yazan kişiyi tespit etmek için paragraf görselini kamerada görünen alanın sol üst köşesine kağıdın tamamı kamera çerçevesi içerisinde olacak şekilde yerleştirilmesi gerekiyor.

Yeni profil eklemek için eklenmek istenen paragraflar siyah kutunun içerisine anlatılan şekilde yerleştirilmeli ve bilgisayarın kamera uygulamasından fotoğraf çekilip uygulama içerisinde Add Profile seçeneği seçilip kamera ile çekilmiş olan fotoğraf veya fotoğraflar seçilmeli son olarak Save Profile butonuna basılmalıdır. Örnek profil examples klasöründe bulunabilir. 

Yeni font eklemek için tablet veya telefondan taslak, verilen karelerin dışına çıkılmayacak biçimde doldurulmalı ve jpg formatında kaydedilmelidir. Font taslağı, klasör içerisinde bulunmaktadır. Uygulama içerisinde Add Font seçeneğine tıklanarak taslak dosyası seçilmeli ve Save Font butonuna basılmalıdır. Örnek font examples klasöründe bulunabilir. 


- Port Erişiminde Problem Olması Durumunda ilgili port için gerekli izinler verilmeli (dev dizinine gidilerek)
	chmod 777 <port>

	örnek senaryo (/dev/tty/USB0):
  ```bash
sudo su
cd .. 
cd ..
cd dev
chmod 777 ttyUSB0
```
