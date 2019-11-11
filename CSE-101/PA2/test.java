void changeEntry(int i, int j, double x) {
    if (i < 1 || i > getSize()) {
      throw new RuntimeException("Matrix Error: ChangeEntry() called on out of bounds i");
    }

    if (j < 1 || j > getSize()) {
      throw new RuntimeException("Matrix Error: ChangeEntry() called on out of bounds j");
    }

    Entry newE = new Entry(j, x);
    boolean inserted = false;
    int mid = row[i].length() / 2;

    //adding new entry to existing List Object in Matrix.
    if (row[i].isEmpty()) {
      if (x != 0) {
        row[i].prepend(newE);
        NNZ++;
      }
      return;
    } else {
      for (row[i].moveFront(); row[i].index() >= 0; row[i].moveNext()) {
        if (((Entry) row[i].get()).col == newE.col) {
          //changing value of initialized Matrix position
          if(x == 0) {
            row[i].delete();
            NNZ--;
            inserted = true;
            return;
          }

          if (x != 0) {
            ((Entry) row[i].get()).val = x;
            return;
          }
        }
        //inserting data at a uninitialized Matrix position
        if (((Entry) row[i].get()).col > j && !inserted) {
          row[i].insertBefore(newE);
          inserted = true;
          NNZ++;
          return;
        }
      }
    }

    if (!inserted && x != 0) {
      row[i].append(newE);
      NNZ++;
    }
  }