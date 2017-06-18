export default class Exercise {

  name;
  sets;
  reps;
  interval;
  weight;
  note;


  constructor(name, sets, reps, interval, weight = null, note = '') {
    this.name     = name;
    this.sets     = sets;
    this.reps     = reps;
    this.interval = interval;
    this.weight   = weight;
    this.note     = note;
  }
}