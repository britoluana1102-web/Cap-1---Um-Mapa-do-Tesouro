#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Definição de Pinos
#define LCD_SDA    21 
#define LCD_SCL    22 
LiquidCrystal_I2C lcd(0x27, 16, 2); 

#define DHT_PIN    17 
#define DHT_TYPE   DHT22
DHT dht(DHT_PIN, DHT_TYPE);

#define PH_PIN     34 
#define PH_MAX     4095.0 

#define BOMBA_PIN  23 

#define N_PIN      18 
#define P_PIN      4  
#define K_PIN      16 

// Variáveis de Estado
const float UMIDADE_MINIMA = 60.0;
float umidade = 0.0;
float temperatura = 0.0;
float pH_Simulado = 0.0;

unsigned long previousMillis = 0; 
const long interval = 2000;      
unsigned long lastButtonPressTime = 0; 

bool n_adicionado = false;
bool p_adicionado = false;
bool k_adicionado = false;

// Modo opcional de decisão por cultura (NPK + pH + umidade).
#define USE_DECISAO_CULTIVO 1

bool FERTIRRIGACAO = true;

struct Cultura {
  const char* nome;
  float umidadeMin;
  float umidadeMax; 
  float phMin;
  float phMax;
  bool exigeN;
  bool exigeP;
  bool exigeK;
};


Cultura culturaSelecionada = {
  "Tomate",  // nome
  65.0,      // umidadeMin (ex.: 65%)
  80.0,      // umidadeMax (ex.: 80%)
  6.0,       // pH mínimo ideal
  6.8,       // pH máximo ideal
  true,      // exige N?
  true,      // exige P?
  true       // exige K?
};

bool decisaoCultivo(bool &motivoUmidade, bool &motivoPH, bool &motivoNPK) {

  motivoPH = (pH_Simulado < culturaSelecionada.phMin) || (pH_Simulado > culturaSelecionada.phMax);

  // Motivo NPK: se cultura exige e ainda não foi "adicionado"
  bool faltaN = culturaSelecionada.exigeN && !n_adicionado;
  bool faltaP = culturaSelecionada.exigeP && !p_adicionado;
  bool faltaK = culturaSelecionada.exigeK && !k_adicionado;
  motivoNPK = (faltaN || faltaP || faltaK);


  bool ph_ok = !motivoPH;
  bool nutr_ok = !FERTIRRIGACAO || !motivoNPK;

  return (motivoUmidade && ph_ok && nutr_ok);
}

// Protótipos
void lerPH();
void controleBomba();
void checarBotoes();
void atualizarLCD();

void setup() {
  Serial.begin(115200);

  Wire.begin(LCD_SDA, LCD_SCL); 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Sistema IoT Agri");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  pinMode(BOMBA_PIN, OUTPUT);
  digitalWrite(BOMBA_PIN, HIGH); 

  pinMode(N_PIN, INPUT_PULLUP);
  pinMode(P_PIN, INPUT_PULLUP);
  pinMode(K_PIN, INPUT_PULLUP);

  dht.begin();
  
  delay(3000); 

#if USE_DECISAO_CULTIVO
  Serial.println(F("=== Modo 'Decisao por Cultura' ATIVADO ==="));
  Serial.print(F("Cultura: ")); Serial.println(culturaSelecionada.nome);
  Serial.print(F("Umid. Min/Max: ")); Serial.print(culturaSelecionada.umidadeMin); 
  Serial.print(F("/")); Serial.println(culturaSelecionada.umidadeMax);
  Serial.print(F("pH ideal: ")); Serial.print(culturaSelecionada.phMin); 
  Serial.print(F(" a ")); Serial.println(culturaSelecionada.phMax);
  Serial.print(F("Exige N/P/K: "));
  Serial.print(culturaSelecionada.exigeN ? "N " : "");
  Serial.print(culturaSelecionada.exigeP ? "P " : "");
  Serial.println(culturaSelecionada.exigeK ? "K" : "");
  Serial.print(F("Fertirrigacao: ")); Serial.println(FERTIRRIGACAO ? "SIM" : "NAO");
#else
  Serial.println(F("=== Modo padrao (umidade DHT22) ==="));
#endif
}

void lerPH() {
  int leituraAnalogica = analogRead(PH_PIN);
  pH_Simulado = map(leituraAnalogica, 0, 4095, 30, 100) / 10.0;
}

void controleBomba() {
#if USE_DECISAO_CULTIVO
  bool mU=false, mPH=false, mNPK=false;
  bool irr = decisaoCultivo(mU, mPH, mNPK);
  digitalWrite(BOMBA_PIN, irr ? LOW : HIGH);

  Serial.print(F("[DECISAO] U=")); Serial.print(umidade, 1); Serial.print(F("% "));
  Serial.print(F("pH=")); Serial.print(pH_Simulado, 1); Serial.print(F("  NPK["));
  Serial.print(n_adicionado ? 'X' : '_');
  Serial.print(p_adicionado ? 'X' : '_');
  Serial.print(k_adicionado ? 'X' : '_');
  Serial.print(F("]  -> IRRIGAR: "));
  Serial.print(irr ? "SIM" : "NAO");
  Serial.print(F("  (motivos: U<min=")); Serial.print(mU ? "S" : "N");
  Serial.print(F(", pH fora="));        Serial.print(mPH ? "S" : "N");
  Serial.print(F(", faltam NPK="));     Serial.print(mNPK ? "S" : "N");
  Serial.println(")");
#else

  if (umidade < UMIDADE_MINIMA && umidade > 0) { 
    digitalWrite(BOMBA_PIN, LOW); 
  } else {
    digitalWrite(BOMBA_PIN, HIGH); 
  }
#endif
}

void checarBotoes() {
  int nState = digitalRead(N_PIN);
  int pState = digitalRead(P_PIN);
  int kState = digitalRead(K_PIN);

  unsigned long currentMillis = millis();

  if (nState == LOW) {
      n_adicionado = true;
      lcd.setCursor(0, 1);
      lcd.print("N Adicionado!   "); 
      lastButtonPressTime = currentMillis;
  }
  
  if (pState == LOW) {
      p_adicionado = true;
      lcd.setCursor(0, 1);
      lcd.print("P Adicionado!   ");
      lastButtonPressTime = currentMillis;
  }
  
  if (kState == LOW) {
      k_adicionado = true;
      lcd.setCursor(0, 1);
      lcd.print("K Adicionado!   ");
      lastButtonPressTime = currentMillis;
  }
}

void atualizarLCD() {
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatura, 1);
  lcd.print("C H:"); 
  lcd.print(umidade, 0);
  lcd.print("% "); 

  if (millis() - lastButtonPressTime > 2000) {
      lcd.setCursor(0, 1);
      lcd.print("pH:");
      lcd.print(pH_Simulado, 1);
      
      lcd.setCursor(6, 1); 
      lcd.print("NPK["); 
      lcd.print(n_adicionado ? 'X' : '_'); 
      lcd.print(p_adicionado ? 'X' : '_'); 
      lcd.print(k_adicionado ? 'X' : '_'); 
      lcd.print("]");

#if USE_DECISAO_CULTIVO
      bool dummyU=false, dummyPH=false, dummyNPK=false;
      bool irr = decisaoCultivo(dummyU, dummyPH, dummyNPK);
      lcd.print(irr ? " IR:S " : " IR:N ");
#else
      lcd.print("  "); 
#endif
  }
}

void loop() {
  checarBotoes();
  controleBomba();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    if (isnan(umidade) || isnan(temperatura)) {
      lcd.setCursor(0, 0);
      lcd.print("Erro DHT22!");
      return;
    }

    lerPH();
    
    atualizarLCD();
  }
}