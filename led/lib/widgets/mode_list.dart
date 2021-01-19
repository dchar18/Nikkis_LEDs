import 'package:flutter/material.dart';
import 'package:led/screens/home_screen.dart';

List<String> modes = [
  "Off",
  "Study",
  "Relax",
  "Party",
  "Christmas twinkle",
  "Blue twinkle",
  "Green twinkle",
  "Snow",
  "Fire",
  "Blue Fire"
];

Widget modeList() {
  return ListView.builder(
    shrinkWrap: true,
    padding: EdgeInsets.only(bottom: 10.0),
    itemCount: modes.length,
    itemBuilder: (BuildContext context, int index) {
      final String mode = modes[index];
      return ModeTile(
        modeName: mode,
      );
    },
  );
}

class ModeTile extends StatelessWidget {
  final String modeName;
  ModeTile({@required this.modeName});

  @override
  Widget build(BuildContext context) {
    // final itemHeight = (MediaQuery.of(context).size.height - 150) / 10;
    final itemHeight = 60.0;

    return GestureDetector(
      onTap: () {
        sendRequest(modeName);
        // ControlScreen.setMode(modeName);
      },
      child: Container(
        padding: EdgeInsets.symmetric(vertical: 10),
        height: itemHeight,
        margin: EdgeInsets.all(10),
        decoration: BoxDecoration(
          color: Colors.blue[50],
          borderRadius: BorderRadius.circular(25),
        ),
        child: Container(
          alignment: Alignment.center,
          child: Text(
            modeName,
            textAlign: TextAlign.center,
            style: TextStyle(
                fontSize: 30,
                color: Colors.grey[800],
                fontWeight: FontWeight.bold),
            overflow: TextOverflow.ellipsis,
          ),
        ),
      ),
    );
  }
}
