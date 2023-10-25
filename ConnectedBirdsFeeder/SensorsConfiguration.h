#define MOCK false // true : allows you to simulate the recovery of measurements without sensors

#define MEASUREMENT_INTERVAL 600000 // Interval between two measurement readings (milliseconds)
#define ID_FEEDER 1 // Identifier of the feeder. It must be unique for all connected feeders that send their data to the same endpoint
#define URL_ENPOINT "http://192.168.0.14:8666/capteurs/releve" // endpoint 
#define DHT_PIN 35   // Pin on which is connected the DHT22 (temperature and humidity sensor)
#define DHT_TYPE DHT22   // DHT sensor type
#define MQ135_PIN 34 // Pin on which the MQ135 is connected (pollution sensor)

#define INFLUXDB_URL "http://163.172.95.137:8086"
#define INFLUXDB_TOKEN "my-super-secret-auth-token"
#define INFLUXDB_ORG "lelieumultiple"
#define INFLUXDB_BUCKET "birdy"