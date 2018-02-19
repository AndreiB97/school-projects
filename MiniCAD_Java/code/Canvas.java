import java.awt.image.BufferedImage;
import static java.awt.image.BufferedImage.TYPE_INT_ARGB;
import static java.lang.Math.abs;
import static java.lang.Math.signum;
import java.util.LinkedList;

public final class Canvas implements Painter {

    ////////////////////////
    // Variables

    private LinkedList<Point> fillQueue;
    private BufferedImage image;
    private final int width;
    private final int height;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public Canvas(final int width, final int height, final int color) {
        this.width = width;
        this.height = height;
        image = new BufferedImage(width, height, TYPE_INT_ARGB);
        fillQueue = new LinkedList<>();

        // fill background
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                image.setRGB(i, j, color);
            }
        }
    }

    public BufferedImage getImage() {
        return image;
    }

    public void draw(final Circle shape) {
        // draw circle using bresenham circle algorithm
        final int codingStyleMagicNumber6 = 6;
        final int codingStyleMagicNumber10 = 10;
        final int codingStyleMagicNumber4 = 4;
        final int codingStyleMagicNumber3 = 3;
        // :)
        Point coord = shape.getCenterCoord();
        int radius = shape.getRadius();
        int outline = shape.getOutlineColor();
        int x = coord.getX();
        int y = coord.getY();
        int p = 0;
        int d = codingStyleMagicNumber3 - 2 * radius;

        while (p <= radius) {
            circleDrawHelper(x, y, p, radius, outline);
            p++;
            if (d <= 0) {
                d += codingStyleMagicNumber4 * p + codingStyleMagicNumber6;
            } else {
                radius--;
                d += codingStyleMagicNumber4 * (p - radius) + codingStyleMagicNumber10;
            }
            circleDrawHelper(x, y, p, radius, outline);
        }
        fillQueue.push(coord);
        while (!fillQueue.isEmpty()) {
            coord = fillQueue.pop();
            fill(outline, shape.getFillColor(), coord.getX(), coord.getY());
        }
    }

    public void circleDrawHelper(final int xCodingStyle, final int yCodingStyle,
                                 final int pCodingStyle, final int qCodingStyle,
                                 final int color) {
        // helper for bresenham circle drawing algorithm
        // #CodingStyleRULZ
        if (xCodingStyle + pCodingStyle >= 0 && xCodingStyle + pCodingStyle < width) {
            if (yCodingStyle + qCodingStyle >= 0 && yCodingStyle + qCodingStyle < height) {
                image.setRGB(xCodingStyle + pCodingStyle, yCodingStyle + qCodingStyle, color);
            }
            if (yCodingStyle - qCodingStyle >= 0 && yCodingStyle - qCodingStyle < height) {
                image.setRGB(xCodingStyle + pCodingStyle, yCodingStyle - qCodingStyle, color);
            }
        }
        if (xCodingStyle + qCodingStyle >= 0 && xCodingStyle + qCodingStyle < width) {
            if (yCodingStyle + pCodingStyle >= 0 && yCodingStyle + pCodingStyle < height) {
                image.setRGB(xCodingStyle + qCodingStyle, yCodingStyle + pCodingStyle, color);
            }
            if (yCodingStyle - pCodingStyle >= 0 && yCodingStyle - pCodingStyle < height) {
                image.setRGB(xCodingStyle + qCodingStyle, yCodingStyle - pCodingStyle, color);
            }
        }
        if (xCodingStyle - pCodingStyle >= 0 && xCodingStyle - pCodingStyle < width) {
            if (yCodingStyle + qCodingStyle >= 0 && yCodingStyle + qCodingStyle < height) {
                image.setRGB(xCodingStyle - pCodingStyle, yCodingStyle + qCodingStyle, color);
            }
            if (yCodingStyle - qCodingStyle >= 0 && yCodingStyle - qCodingStyle < height) {
                image.setRGB(xCodingStyle - pCodingStyle, yCodingStyle - qCodingStyle, color);
            }
        }
        if (xCodingStyle - qCodingStyle >= 0 && xCodingStyle - qCodingStyle < width) {
            if (yCodingStyle + pCodingStyle >= 0 && yCodingStyle + pCodingStyle < height) {
                image.setRGB(xCodingStyle - qCodingStyle, yCodingStyle + pCodingStyle, color);
            }
            if (yCodingStyle - pCodingStyle >= 0 && yCodingStyle - pCodingStyle < height) {
                image.setRGB(xCodingStyle - qCodingStyle, yCodingStyle - pCodingStyle, color);
            }
        }
    }

    public void draw(final Diamond shape) {
        // draw diamong
        Point coord = shape.getCenterCoord();
        int horizontal = shape.getHorizontalDiagonalLength();
        int vertical = shape.getVerticalDiagonalLength();
        int outline = shape.getOutlineColor();
        int x = coord.getX();
        int y = coord.getY();

        // draw lines a corner of the diamong to the next corner
        draw(new Line(x, y + vertical / 2, x + horizontal / 2, y, outline));
        draw(new Line(x + horizontal / 2, y, x, y - vertical / 2, outline));
        draw(new Line(x, y - vertical / 2, x - horizontal / 2, y, outline));
        draw(new Line(x - horizontal / 2, y, x, y + vertical / 2, outline));

        // add center to the fill queue
        fillQueue.push(new Point(x, y));
        while (!fillQueue.isEmpty()) {
            // fill the pixels while queue not empty
            coord = fillQueue.pop();
            fill(outline, shape.getFillColor(), coord.getX(), coord.getY());
        }
    }

    public void draw(final Line shape) {
        // draw line using bresenham line algorithm
        Point[] coord = shape.getCoord();
        int x1 = coord[0].getX();
        int y1 = coord[0].getY();
        int x2 = coord[1].getX();
        int y2 = coord[1].getY();

        int deltaX = abs(x2 - x1);
        int deltaY = abs(y2 - y1);
        int s1 = (int) signum(x2 - x1);
        int s2 = (int) signum(y2 - y1);
        boolean interchanged;

        if (deltaY > deltaX) {
            deltaX += deltaY;
            deltaY = deltaX - deltaY;
            deltaX -= deltaY;
            interchanged = true;
        } else {
            interchanged = false;
        }

        int error = 2 * deltaY - deltaX;

        for (int i = 0; i <= deltaX; i++) {
            if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
                image.setRGB(x1, y1, shape.getColor());
            }

            while (error > 0) {
                if (interchanged) {
                    x1 += s1;
                } else {
                    y1 += s2;
                }

                error -= 2 * deltaX;
            }

            if (interchanged) {
                y1 += s2;
            } else {
                x1 += s1;
            }

            error += 2 * deltaY;
        }
    }

    public void draw(final Polygon shape) {
        // draw a polygon
        Point[] coord = shape.getCoord();
        int number = shape.getPointNumber();
        int outline = shape.getOutlineColor();
        int x = coord[0].getX(), y = coord[0].getY();

        // draw a line from a corner of the polygon to the next
        for (int i = 0; i < number - 1; i++) {
            draw(new Line(coord[i].getX(), coord[i].getY(), coord[i + 1].getX(),
                    coord[i + 1].getY(), outline));
            x += coord[i + 1].getX();
            y += coord[i + 1].getY();
        }

        draw(new Line(coord[number - 1].getX(), coord[number - 1].getY(), coord[0].getX(),
                coord[0].getY(), outline));

        // add center of the polygon to the fill queue
        fillQueue.push(new Point(x / number, y / number));
        while (!fillQueue.isEmpty()) {
            // fill pixels while queue not empty
            Point tmp;
            tmp = fillQueue.pop();
            fill(outline, shape.getFillColor(), tmp.getX(), tmp.getY());
        }
    }

    public void draw(final Rectangle shape) {
        // draw rectangle
        Point coord = shape.getUpperLeftCoord();
        int codingStyleWidth = shape.getWidth();
        int codingStyleHeight = shape.getHeight();
        int outline = shape.getOutlineColor();
        int x = coord.getX();
        int y = coord.getY();

        // draw a line from a corner of the rectangle to the next corner
        draw(new Line(x, y, x + codingStyleWidth - 1, y, outline));
        draw(new Line(x + codingStyleWidth - 1, y, x + codingStyleWidth - 1,
                y + codingStyleHeight - 1, outline));
        draw(new Line(x + codingStyleWidth - 1, y + codingStyleHeight - 1, x,
                y + codingStyleHeight - 1, outline));
        draw(new Line(x, y + codingStyleHeight - 1, x, y, outline));

        // fill the pixels
        for (int i = x + 1; i < x + codingStyleWidth - 1; i++) {
            for (int j = y + 1; j < y + codingStyleHeight - 1; j++) {
                if (i >= 0 && i < this.width && j >= 0 && j < this.height) {
                    image.setRGB(i, j, shape.getFillColor());
                }
            }
        }
    }

    public void draw(final Square shape) {
        // draw a square
        Point coord = shape.getUpperLeftCoord();
        int length = shape.getLength();
        int outline = shape.getOutlineColor();
        int x = coord.getX();
        int y = coord.getY();

        // draw a line from a corner of the square to the next
        draw(new Line(x, y, x + length - 1, y, outline));
        draw(new Line(x + length - 1, y, x + length - 1, y + length - 1, outline));
        draw(new Line(x + length - 1, y + length - 1, x, y + length - 1, outline));
        draw(new Line(x, y + length - 1, x, y, outline));

        // fill pixels
        for (int i = x + 1; i < x + length - 1; i++) {
            for (int j = y + 1; j < y + length - 1; j++) {
                if (i >= 0 && i < width && j >= 0 && j < height) {
                    image.setRGB(i, j, shape.getFillColor());
                }
            }
        }
    }

    public void draw(final Triangle shape) {
        // draw a triangle
        Point[] coord = shape.getCoord();
        final int codingStyleMagicNumber3 = 3;
        int outline = shape.getOutlineColor();
        int[] x = new int[codingStyleMagicNumber3];
        int[] y = new int[codingStyleMagicNumber3];
        for (int i = 0; i < codingStyleMagicNumber3; i++) {
            x[i] = coord[i].getX();
            y[i] = coord[i].getY();
        }

        // draw a line from a corner of the triangle to the next
        draw(new Line(x[0], y[0], x[1], y[1], outline));
        draw(new Line(x[1], y[1], x[2], y[2], outline));
        draw(new Line(x[2], y[2], x[0], y[0], outline));

        // add center of the triangle to the fill queue
        fillQueue.push(new Point((x[0] + x[1] + x[2]) / codingStyleMagicNumber3,
                (y[0] + y[1] + y[2]) / codingStyleMagicNumber3));

        while (!fillQueue.isEmpty()) {
            // fill pixels while queue not empty
            Point tmp = fillQueue.pop();
            fill(outline, shape.getFillColor(), tmp.getX(), tmp.getY());
        }
    }

    public void fill(final int outlineColor, final int fillColor, final int x, final int y) {
        // flood fill
        if (x >= 0 && x < width && y >= 0 && y < height) {
            if (image.getRGB(x, y) != outlineColor && image.getRGB(x, y) != fillColor) {
                // if can fill then fill and add up/down/left/right pixels to the queue
                image.setRGB(x, y, fillColor);
                fillQueue.push(new Point(x + 1, y));
                fillQueue.push(new Point(x - 1, y));
                fillQueue.push(new Point(x, y + 1));
                fillQueue.push(new Point(x, y - 1));
            }
        }
    }

    // End methods
    ////////////////////////
}
