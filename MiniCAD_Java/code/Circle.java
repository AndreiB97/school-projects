public final class Circle implements DrawableShape {

    ////////////////////////
    // Variables

    private final Point centerCoord;
    private final int radius;
    private final int outlineColor;
    private final int fillColor;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public Point getCenterCoord() {
        return centerCoord;
    }

    public int getRadius() {
        return radius;
    }

    public int getOutlineColor() {
        return outlineColor;
    }

    public int getFillColor() {
        return fillColor;
    }

    public Circle(final int x, final int y, final int radius, final int outlineColor,
                  final int fillColor) {
        centerCoord = new Point(x, y);
        this.radius = radius;
        this.outlineColor = outlineColor;
        this.fillColor = fillColor;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////
}
