import java.util.Scanner;

public final class ShapeFactory {

    ////////////////////////
    // Variables

    private static final ShapeFactory FACTORY = new ShapeFactory();

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public static ShapeFactory getInstance() {
        return FACTORY;
    }

    public DrawableShape getShape(final String shapeName, final Scanner scan) {
        int outline, fill, alpha;
        int[] x, y;
        int length;
        int radius;
        int width, height;
        int horizontal, vertical;
        int pointNumber;
        String rgb;

        switch (shapeName) {
            case "SQUARE":
                x = new int[1];
                y = new int[1];
                x[0] = scan.nextInt();
                y[0] = scan.nextInt();
                length = scan.nextInt();
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);
                rgb = scan.next();
                alpha = scan.nextInt();
                fill = Main.getRGBAValue(rgb, alpha);

                return new Square(x[0], y[0], length, outline, fill);
            case "LINE":
                x = new int[2];
                y = new int[2];
                x[0] = scan.nextInt();
                y[0] = scan.nextInt();
                x[1] = scan.nextInt();
                y[1] = scan.nextInt();
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);

                return new Line(outline, x, y);
            case "CIRCLE":
                x = new int[1];
                y = new int[1];
                x[0] = scan.nextInt();
                y[0] = scan.nextInt();
                radius = scan.nextInt();
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);
                rgb = scan.next();
                alpha = scan.nextInt();
                fill = Main.getRGBAValue(rgb, alpha);

                return new Circle(x[0], y[0], radius, outline, fill);
            case "DIAMOND":
                x = new int[1];
                y = new int[1];
                x[0] = scan.nextInt();
                y[0] = scan.nextInt();
                horizontal = scan.nextInt();
                vertical = scan.nextInt();
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);
                rgb = scan.next();
                alpha = scan.nextInt();
                fill = Main.getRGBAValue(rgb, alpha);

                return new Diamond(x[0], y[0], horizontal, vertical, outline, fill);
            case "POLYGON":
                pointNumber = scan.nextInt();
                x = new int[pointNumber];
                y = new int[pointNumber];
                for (int i = 0; i < pointNumber; i++) {
                    x[i] = scan.nextInt();
                    y[i] = scan.nextInt();
                }
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);
                rgb = scan.next();
                alpha = scan.nextInt();
                fill = Main.getRGBAValue(rgb, alpha);

                return new Polygon(pointNumber, x, y, outline, fill);
            case "RECTANGLE":
                x = new int[1];
                y = new int[1];
                x[0] = scan.nextInt();
                y[0] = scan.nextInt();
                height = scan.nextInt();
                width = scan.nextInt();
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);
                rgb = scan.next();
                alpha = scan.nextInt();
                fill = Main.getRGBAValue(rgb, alpha);

                return new Rectangle(x[0], y[0], width, height, outline, fill);
            case "TRIANGLE":
                final int codingStyleMagicNumber3 = 3;
                // :)
                x = new int[codingStyleMagicNumber3];
                y = new int[codingStyleMagicNumber3];
                for (int i = 0; i < codingStyleMagicNumber3; i++) {
                    x[i] = scan.nextInt();
                    y[i] = scan.nextInt();
                }
                rgb = scan.next();
                alpha = scan.nextInt();
                outline = Main.getRGBAValue(rgb, alpha);
                rgb = scan.next();
                alpha = scan.nextInt();
                fill = Main.getRGBAValue(rgb, alpha);

                return new Triangle(x, y, outline, fill);
            default:
                return null;
        }
    }

    // End methods
    ////////////////////////

}
