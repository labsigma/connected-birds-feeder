#define MEASUREMENT_INTERVAL 60 // Time ESP32 will go to sleep (in seconds)
#define ID_FEEDER 1 // Identifier of the feeder. It must be unique for all connected feeders that send their data to the same endpoint
#define DHT_PIN 35   // Pin on which is connected the DHT22 (temperature and humidity sensor)
#define DHT_TYPE DHT22   // DHT sensor type
#define MQ135_PIN 34 // Pin on which the MQ135 is connected (pollution sensor)

#define INFLUXDB_URL "http://163.172.95.137:8086"
#define INFLUXDB_TOKEN "my-super-secret-auth-token"
#define INFLUXDB_ORG "lelieumultiple"
#define INFLUXDB_BUCKET "birdy"

#define HTTP_TIMEOUT 30000

#define IA_RECOGNITION_API "http://163.172.95.137:5001/api/pictures"
#define IA_RECOGNITION_USERNAME "my-user"
#define IA_RECOGNITION_PASSWORD "my-password"

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
#define JPEG_QUALITY      10
#define FB_COUNT          2

// PIN on whith the presence detector module is connected.
#define PIR_PIN  GPIO_NUM_33 //  Only RTC IO can be used as a source for external wake source. They are pins: 0,2,4,12-15,25-27,32-39.