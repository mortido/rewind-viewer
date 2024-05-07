import math


class colors:
    EPS = 1e-7

    @staticmethod
    def heat_color(value, min_value, max_value):
        value = min(max(value, min_value), max_value - colors.EPS)
        d = max_value - min_value
        value = 0.5 if d < colors.EPS else (value - min_value) / d
        m = 0.25
        range = math.floor(value / m)
        s = (value - range * m) / m
        r = g = b = 0

        if range == 0:
            r, g, b = 0, s, 1
        elif range == 1:
            r, g, b = 0, 1, 1 - s
        elif range == 2:
            r, g, b = s, 1, 0
        elif range == 3:
            r, g, b = 1, 1 - s, 0

        R = int(255 * r)
        G = int(255 * g)
        B = int(255 * b)

        return (R << 16) | (G << 8) | B

    class red:
        IndianRed = 0xCD5C5C
        LightCoral = 0xF08080
        Salmon = 0xFA8072
        DarkSalmon = 0xE9967A
        LightSalmon = 0xFFA07A
        Crimson = 0xDC143C
        Red = 0xFF0000
        FireBrick = 0xB22222
        DarkRed = 0x8B0000

    class pink:
        Pink = 0xFFC0CB
        LightPink = 0xFFB6C1
        HotPink = 0xFF69B4
        DeepPink = 0xFF1493
        MediumVioletRed = 0xC71585
        PaleVioletRed = 0xDB7093

    class orange:
        LightSalmon = 0xFFA07A
        Coral = 0xFF7F50
        Tomato = 0xFF6347
        OrangeRed = 0xFF4500
        DarkOrange = 0xFF8C00
        Orange = 0xFFA500

    class yellow:
        Gold = 0xFFD700
        Yellow = 0xFFFF00
        LightYellow = 0xFFFFE0
        LemonChiffon = 0xFFFACD
        LightGoldenrodYellow = 0xFAFAD2
        PapayaWhip = 0xFFEFD5
        Moccasin = 0xFFE4B5
        PeachPuff = 0xFFDAB9
        PaleGoldenrod = 0xEEE8AA
        Khaki = 0xF0E68C
        DarkKhaki = 0xBDB76B

    class purple:
        Lavender = 0xE6E6FA
        Thistle = 0xD8BFD8
        Plum = 0xDDA0DD
        Violet = 0xEE82EE
        Orchid = 0xDA70D6
        Fuchsia = 0xFF00FF
        Magenta = 0xFF00FF
        MediumOrchid = 0xBA55D3
        MediumPurple = 0x9370DB
        RebeccaPurple = 0x663399
        BlueViolet = 0x8A2BE2
        DarkViolet = 0x9400D3
        DarkOrchid = 0x9932CC
        DarkMagenta = 0x8B008B
        Purple = 0x800080
        Indigo = 0x4B0082
        SlateBlue = 0x6A5ACD
        DarkSlateBlue = 0x483D8B
        MediumSlateBlue = 0x7B68EE

    class green:
        GreenYellow = 0xADFF2F
        Chartreuse = 0x7FFF00
        LawnGreen = 0x7CFC00
        Lime = 0x00FF00
        LimeGreen = 0x32CD32
        PaleGreen = 0x98FB98
        LightGreen = 0x90EE90
        MediumSpringGreen = 0x00FA9A
        SpringGreen = 0x00FF7F
        MediumSeaGreen = 0x3CB371
        SeaGreen = 0x2E8B57
        ForestGreen = 0x228B22
        Green = 0x008000
        DarkGreen = 0x006400
        YellowGreen = 0x9ACD32
        OliveDrab = 0x6B8E23
        Olive = 0x808000
        DarkOliveGreen = 0x556B2F
        MediumAquamarine = 0x66CDAA
        DarkSeaGreen = 0x8FBC8B
        LightSeaGreen = 0x20B2AA
        DarkCyan = 0x008B8B
        Teal = 0x008080

    class blue:
        Aqua = 0x00FFFF
        Cyan = 0x00FFFF
        LightCyan = 0xE0FFFF
        PaleTurquoise = 0xAFEEEE
        Aquamarine = 0x7FFFD4
        Turquoise = 0x40E0D0
        MediumTurquoise = 0x48D1CC
        DarkTurquoise = 0x00CED1
        CadetBlue = 0x5F9EA0
        SteelBlue = 0x4682B4
        LightSteelBlue = 0xB0C4DE
        PowderBlue = 0xB0E0E6
        LightBlue = 0xADD8E6
        SkyBlue = 0x87CEEB
        LightSkyBlue = 0x87CEFA
        DeepSkyBlue = 0x00BFFF
        DodgerBlue = 0x1E90FF
        CornflowerBlue = 0x6495ED
        MediumSlateBlue = 0x7B68EE
        RoyalBlue = 0x4169E1
        Blue = 0x0000FF
        MediumBlue = 0x0000CD
        DarkBlue = 0x00008B
        Navy = 0x000080
        MidnightBlue = 0x191970

    class brown:
        Cornsilk = 0xFFF8DC
        BlanchedAlmond = 0xFFEBCD
        Bisque = 0xFFE4C4
        NavajoWhite = 0xFFDEAD
        Wheat = 0xF5DEB3
        BurlyWood = 0xDEB887
        Tan = 0xD2B48C
        RosyBrown = 0xBC8F8F
        SandyBrown = 0xF4A460
        Goldenrod = 0xDAA520
        DarkGoldenrod = 0xB8860B
        Peru = 0xCD853F
        Chocolate = 0xD2691E
        SaddleBrown = 0x8B4513
        Sienna = 0xA0522D
        Brown = 0xA52A2A
        Maroon = 0x800000

    class white:
        White = 0xFFFFFF
        Snow = 0xFFFAFA
        HoneyDew = 0xF0FFF0
        MintCream = 0xF5FFFA
        Azure = 0xF0FFFF
        AliceBlue = 0xF0F8FF
        GhostWhite = 0xF8F8FF
        WhiteSmoke = 0xF5F5F5
        SeaShell = 0xFFF5EE
        Beige = 0xF5F5DC
        OldLace = 0xFDF5E6
        FloralWhite = 0xFFFAF0
        Ivory = 0xFFFFF0
        AntiqueWhite = 0xFAEBD7
        Linen = 0xFAF0E6
        LavenderBlush = 0xFFF0F5
        MistyRose = 0xFFE4E1

    class gray:
        Gainsboro = 0xDCDCDC
        LightGray = 0xD3D3D3
        Silver = 0xC0C0C0
        DarkGray = 0xA9A9A9
        Gray = 0x808080
        DimGray = 0x696969
        LightSlateGray = 0x778899
        SlateGray = 0x708090
        DarkSlateGray = 0x2F4F4F
        Black = 0x000000
