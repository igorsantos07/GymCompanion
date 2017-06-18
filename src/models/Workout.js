export default class Workout {

  static lastLetter = 64;

  letter;
  name;

  constructor(name = null) {
    this.letter = String.fromCharCode(++Workout.lastLetter);
    this.name   = name || 'Workout ' + this.letter;
  }
}