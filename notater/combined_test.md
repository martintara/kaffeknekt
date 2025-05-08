27.apr
Testet combined test, fikk ikke til trykksensor. tbc i morgen lul


3.may
suspekt timing, ikke et enormt problem, men burde testet.
![[combined_test_timing.png]]

may 5.
step by step integrering
![[combined_test_step_by_step.png]]
Rounding issue. Fix: change int to float 
![[combined_test_float_issue.png]]
![[combined_test_float_corrected.png]]
![[combined_test_korrekt_output.png]]

5. may
Feil var konflikt med pins. Vi hadde brukt 21 som SDA.
![[combined_test_sda_konflikt.png]]

may 6.
Implemented ACPower task with static dummy data.
![[combined_test_acpower_static.png]]

Problemer med å implementere ads (ac wattmeter):
![[combined_test_assert.png]]


may 7
Mutex fikset dette.

Timing problem:
![[combined_test_timing_prob.png]]

Jobbet en del med brew trigger. Må testest mer da maskinen er intakt.

Implementerte temperature task med statisk data:
![[combined_test_temperature_static.png]]

Implementerte temp sensoren:
![[combined_test_temp_implemented.png]]

implementerte trykk sensor med statisk data:
![[combined_test_pressure_static.png]]

i2c feil igjen
![[combined_test_pressure_i2c_fault.png]]

implementert riktig med mutex. Discovery: Wire bruker i2c også
![[combined_test_pressure_implemented_correctly.png]]

may 8:
brew trigger live test. works. could be improved with timing tasks. maybe seperating out the watt meters in different tasks, give priority to brew trigger wattmeter.
observation: timing skips a beat, should look into the rtc task

![[combined_test_live_trigger_test.png]]