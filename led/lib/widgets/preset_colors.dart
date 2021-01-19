import 'package:flutter/material.dart';
import 'package:led/screens/home_screen.dart';

List<Color> presets = [
  Color(0x9ffb0022), // pink (155,0,34)
  Color(0xfffc0a00), // blood red (252,10,0)
  Color(0xffff0000),
  Color(0xffff0022), // hot pink (255,0,34)
  Color(0xffff2b00), // orange (255,43,0)
  Color(0xffff2b0a), // peach (255,43,10)
  Color(0xff50ff41), // mint green (80,255,65)
  Color(0xff00ff00),
  Color(0xff00ffff), // aqua blue (0,255,255)
  Color(0xff0fd0ff), // light blue (15,208,255)
  Color(0xff0000ff),
  Color(0xff1560ff), // metal blue (21,96,255)
  Color(0xffff00ff), // purple (255,0,255)
];

presetColors(BuildContext context) {
  return Container(
    height: 60,
    width: MediaQuery.of(context).size.width,
    padding: EdgeInsets.all(10),
    child: ListView.builder(
      scrollDirection: Axis.horizontal,
      shrinkWrap: true,
      // padding: EdgeInsets.only(top: 15.0),
      itemCount: presets.length,
      itemBuilder: (BuildContext context, int index) {
        final Color color = presets[index];
        return ColorTile(
          color: color,
        );
      },
    ),
  );
}

class ColorTile extends StatelessWidget {
  final Color color;
  ColorTile({@required this.color});

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {},
      child: Container(
        padding: EdgeInsets.all(10),
        margin: EdgeInsets.only(left: 5, right: 5),
        height: 40,
        width: 40,
        decoration: BoxDecoration(
            color: color,
            borderRadius: BorderRadius.all(Radius.circular(20)),
            border: Border.all(
                width: 4, color: Colors.white, style: BorderStyle.solid)),
      ),
    );
  }
}
