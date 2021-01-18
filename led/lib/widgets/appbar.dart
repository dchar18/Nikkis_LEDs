import 'package:flutter/material.dart';

Widget myAppBar(BuildContext context, String title) {
  return AppBar(
    backgroundColor: Theme.of(context).appBarTheme.color,
    title: Text(
      title,
      style: TextStyle(fontSize: 25.0, fontWeight: FontWeight.bold),
    ),
  );
}
