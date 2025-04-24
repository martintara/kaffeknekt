option task = {name: "event_log", every: 15m}

send1 = from(bucket: "sensor_data")
    |> range(start: -task.every)
    |> filter(fn: (r) => r._measurement == "Esp32Metrics" and r._field == "power_mW")
    |> filter(fn: (r) => r._value > 0)
    |> first()
    |> findRecord(fn: (key) => true, idx: 0)
    |> (r) => r._time
    
send = date.sub(from: send1, d: 5s)
until = date.add(d: 30s, to: send1)

from (bucket: "sensor_data")
    |> range(start: send, stop: until)
    |> filter(fn: (r) => r._measurement == "Esp32Metrics")
    |> to(bucket: "event_data") 
