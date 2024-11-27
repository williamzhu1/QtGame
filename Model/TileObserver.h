#ifndef TILEOBSERVER_H
#define TILEOBSERVER_H
class TileObserver {
public:
    virtual ~TileObserver() {}
    virtual void onTileChanged() = 0;
};

#endif // TILEOBSERVER_H
