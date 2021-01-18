import 'package:flutter/material.dart';

sliderText(String title, double val, Color color) {
  return Container(
    alignment: Alignment.centerLeft,
    padding: EdgeInsets.only(left: 15),
    child: Row(
      children: [
        Text(
          title + ": ",
          style: TextStyle(
              color: color,
              fontSize: 25,
              fontWeight: FontWeight.bold,
              fontFamily: 'Roboto'),
        ),
        Text(
          val.round().toString(),
          style: TextStyle(
              color: color,
              fontSize: 25,
              // fontWeight: FontWeight.bold,
              fontFamily: 'Roboto'),
        ),
      ],
    ),
  );
}
