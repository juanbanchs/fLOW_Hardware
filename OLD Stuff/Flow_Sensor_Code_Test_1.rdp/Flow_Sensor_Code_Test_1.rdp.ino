int X;
int Y;
float TIME = 0;
float FREQUENCY = 0;
float WATER = 0;
float TOTAL = 0;
float LS = 0;
const int input = A0;

void setup()
{
  Serial.begin(9600);
  Serial.print("Water Flow Meter");
  Serial.print('\n');
  Serial.print("****************");
  delay(1000);
  Serial.print('\n');
  Serial.print('\n');
  pinMode(input,INPUT);
}

void loop()
{
X = pulseIn(input, HIGH);
Y = pulseIn(input, LOW);
TIME = X + Y;
FREQUENCY = 1000000/TIME;
WATER = FREQUENCY/7.5;
LS = WATER/60;
if(FREQUENCY >= 0)
{
  if(isinf(FREQUENCY))
  {
    Serial.print("VOL. :0.00");
    Serial.print('\n');
    Serial.print("TOTAL:");
    Serial.print( TOTAL);
    Serial.print(" L");
    Serial.print('\n');
    Serial.print('\n');
  }
  else
  {
    TOTAL = TOTAL + LS;
    Serial.print("In loop B");
    Serial.print(FREQUENCY);
    Serial.print('\n');
    Serial.print("VOL.: ");
    Serial.print(WATER);
    Serial.print(" L/M");
    Serial.print('\n');
    Serial.print("TOTAL:");
    Serial.print( TOTAL);
    Serial.print(" L");
    Serial.print('\n');
    Serial.print('\n');
  }
  }
  delay(1000);
  }
