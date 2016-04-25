The keyboard is muxed with the LCD data lines.
To read the keyboard, set the LCD data lines as input and enable a weak pull-down.
Then, set the three keyboard lines K1, K2, K3 high one at a time, and watch for which data lines go high.

Here is the keyboard matrix:

|        | **K1** | **K2** | **K3** |
|--------|--------|--------|--------|
| **D0** |   1    |  Green |        |
| **D1** |   2    |  Up    |        |
| **D2** |   3    |  Down  |        |
| **D3** |   4    |  7     |        |
| **D4** |   5    |  8     |        |
| **D5** |   6    |  9     |        |
| **D6** |   0    |  #     | Side   |
| **D7** |   *    |  Red   | Moni   |
