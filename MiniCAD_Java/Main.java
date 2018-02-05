import javax.imageio.ImageIO;
import java.io.File;
import java.util.Scanner;
import java.io.IOException;

public class Main {

    ////////////////////////
    // Methods

    public static int getRGBAValue(final String rgb, final int alpha) {
        // get rgba value of a color
        int rgba = alpha;
        final int codingStyleMagicNumber24 = 24;
        final int codingStyleMagicNumber16 = 16;
        // :)
        rgba = rgba << codingStyleMagicNumber24;
        rgba += Integer.parseInt(rgb.substring(1), codingStyleMagicNumber16);
        return rgba;
    }

    public static void main(final String[] args) {
        try {
            // init the scanner
            final Scanner scan = new Scanner(new File(args[0]));
            final int n = scan.nextInt();
            String shapeName;

            // scan canvas
            shapeName = scan.next();

            if (shapeName.equals("CANVAS")) {

                // get size
                final int width, height;
                height = scan.nextInt();
                width = scan.nextInt();

                // get background color
                final String rgb = scan.next();
                final int alpha = scan.nextInt();
                final int rgba = getRGBAValue(rgb, alpha);

                final Canvas canvas = new Canvas(width, height, rgba);

                // get shape and draw shape
                for (int i = 1; i < n; i++) {
                    shapeName = scan.next();
                    final DrawableShape newShape =
                            ShapeFactory.getInstance().getShape(shapeName, scan);
                    newShape.getDrawn(canvas);
                }

                // write image
                ImageIO.write(canvas.getImage(), "png", new File("drawing.png"));
            } else {
                System.out.println("ERROR: No Canvas");
            }
        } catch (IOException e) {
            System.out.println("ERROR: Cannot open input file");
        }

    }

    // End methods
    ////////////////////////

}
