# Azerbaijan Flag
# Blue:  0x0092BC (top third)
# Red:   0xE8112D (middle third)
# Green: 0x00B050 (bottom third)
# White: 0xFFFFFF (crescent + star)
# Width=350, Height=260, each stripe = ~87 rows

# ── BLUE stripe (y: 0 → 86) ──────────────────────────
li t0, 1
sw t0, 0x0(zero)           # CMD_TYPE = FILL
sw zero, 0x4(zero)         # X1 = 0
sw zero, 0x8(zero)         # Y1 = 0
li t0, 349
sw t0, 0xC(zero)           # X2 = 349
li t0, 86
sw t0, 0x10(zero)          # Y2 = 86
li t0, 0x0092BCFF
sw t0, 0x14(zero)          # COLOR = blue
li t0, 1
sw t0, 0x18(zero)          # START

# ── RED stripe (y: 87 → 173) ─────────────────────────
li t0, 1
sw t0, 0x0(zero)
sw zero, 0x4(zero)
li t0, 87
sw t0, 0x8(zero)           # Y1 = 87
li t0, 349
sw t0, 0xC(zero)
li t0, 173
sw t0, 0x10(zero)          # Y2 = 173
li t0, 0xE8112DFF
sw t0, 0x14(zero)          # COLOR = red
li t0, 1
sw t0, 0x18(zero)          # START

# ── GREEN stripe (y: 174 → 259) ──────────────────────
li t0, 1
sw t0, 0x0(zero)
sw zero, 0x4(zero)
li t0, 174
sw t0, 0x8(zero)           # Y1 = 174
li t0, 349
sw t0, 0xC(zero)
li t0, 259
sw t0, 0x10(zero)          # Y2 = 259
li t0, 0x00B050FF
sw t0, 0x14(zero)          # COLOR = green
li t0, 1
sw t0, 0x18(zero)          # START