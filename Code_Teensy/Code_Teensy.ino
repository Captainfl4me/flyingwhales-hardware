void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //Reception radio (tableau de 3 chiffres deltaX,Y,Z)

  
  //partie Calcul mouvement plot
  //regardez millis() et déduire l'avancer réel de la pince
  // ajouter avancer réel à la position de la pince

  //update la target avec valeur si réçu ?

  //Recalcul les longueurs de fils

  //Calcul delta D = longueur th - longueur réel

  //Avec max(deltaD) on calcul le deltaT avec la vitesseMax
  //(Attention règler vitesse Max en fonction du deltaMillis moyen pour pas overshoot trop)
  //calcul des 3 autres vitesses de moteur avec le deltaT (tous les moteurs arrivent à l'objectif en même temps)

  //Application des nouvelles  vitesse
  //sauvegarde du temps debut init
}
