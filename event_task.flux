import "date"
import "contrib/tomhollingworth/events"
import "join"

option task = {name: "event_log", every: 1m}

// tid = from(bucket: "sensor_data")
   // |> range(start: -task.every)
    // check for table and parameter 
   // |> filter(fn: (r) => r._measurement == "Esp32Metrics" and r._field == "power_mW")
    // check if value > 0
   // |> filter(fn: (r) => r._value > 0)
   // |> first()
   // |> findRecord(fn: (key) => true, idx: 0)
    // from that record, get its time
   // |> (r) => r._time

kaffe = from(bucket: "sensor_data")
   |> range(start: -2m)
   |> filter(fn: (r) => 
         r._measurement == "Esp32Metrics"
      )
   //|> pivot(rowKey: ["_time"], columnKey: ["_field"], valueColumn: "_value")
   |> filter(fn: (r) => 
         r._field == "flag"
      )
   |> events.duration(
         column: "_value",
         timeColumn: "_time",
         //columnName: "duration",
         //stopColumn: "_stop",
         unit: 1s, 
         //start: (r) => r._value == "U", 
         //stop: (r) => r._value == "D")
   |> map(fn: (r) => ({
         _time: r._time, 
         //session_id: readable_time, 
         send: date.sub(from: r._start, d: 5s), 
         until: date.add(to: r._stop, d: 10s), 
         _measurement: r._measurement
       }))

monitor = from(bucket: "sensor_data")
   |> range(start: -2m)
   |> filter(fn: (r) => 
         r._measurement == "Esp32Metrics"
      )
   |> drop(columns: [
      //"readable_time", 
      "_measurement", 
      "_start", 
      "_stop"
      ])

combine = join.left(
   left: kaffe,
   right: monitor,
   on: (l, r) => l._time == r._time, 
   as: (l, r) => ({
      l with 
      temperature: r.temperature, 
      power: r.power, 
      pressure: r.pressure, 
      session_ID: r.readable_time
}))

combine
   |> group(columns: ["_time"])
   |> to(bucket: "event_data", org: "Kaffeknekt")

      

//send = date.sub(from: tid, d: 5s)
//until = date.add(to: tid, d: 10s)

//from (bucket: "sensor_data")
//    |> range(start: send, stop: until)
//    |> filter(fn: (r) => r._measurement == "Esp32Metrics")
//    |> to(bucket: "event_data", org: "Kaffeknekt") 


// prøvde å extracte hver u og d manuelt
//upspike = 
//   from(bucket: "sensor_data")
//      |> range(start: -2m)
//      |> filter(fn: (r) => 
//            r._measurement == "Esp32Metrics and
//            r._field == "flag" and
//            r._value == "U")
//      |> map(fn: (r) => ({
//         _time: r._time
//         send: date.sub(from: r._time, d: 5s)})

//downspike = 
//   from(bucket: "sensor_data")
//      |> range(start: -2m)
//      |> filter(fn: (r) => 
//            r._measurement == "Esp32Metrics and
//            r._field == "flag" and
//            r._value == "D")
//      |> map(fn: (r) => ({
//         _time: r._time
//         until: date.add(from: r._time, d: 5s)})
         
